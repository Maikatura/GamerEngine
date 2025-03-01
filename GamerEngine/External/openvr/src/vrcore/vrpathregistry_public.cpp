//========= Copyright Valve Corporation ============//

#include <vrcore/vrpathregistry_public.h>
#include <json/json.h>
#include <vrcore/pathtools_public.h>
#include <vrcore/envvartools_public.h>
#include <vrcore/strtools_public.h>
#include <vrcore/dirtools_public.h>

#if defined( WIN32 )
#include <windows.h>
#include <shlobj.h>

#undef GetEnvironmentVariable
#elif defined OSX
#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#elif defined(LINUX)
#include <dlfcn.h>
#include <stdio.h>
#endif

#include <algorithm>
#include <sstream>

#ifndef VRLog
	#if defined( __MINGW32__ )
		#define VRLog(args...)		fprintf(stderr, args)
	#elif defined( WIN32 )
		#define VRLog(fmt, ...)		fprintf(stderr, fmt, __VA_ARGS__)
	#else
		#define VRLog(args...)		fprintf(stderr, args)
	#endif
#endif

/** Returns the root of the directory the system wants us to store user config data in */
static std::string GetAppSettingsPath()
{
#if defined( WIN32 )
	WCHAR rwchPath[MAX_PATH];

	if( !SUCCEEDED( SHGetFolderPathW( NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, rwchPath ) ) )
	{
		return "";
	}

	// Convert the path to UTF-8 and store in the output
	std::string sUserPath = UTF16to8( rwchPath );

	return sUserPath;
#elif defined( OSX )
	std::string sSettingsDir;
	@autoreleasepool {
		// Search for the path
		NSArray *paths = NSSearchPathForDirectoriesInDomains( NSApplicationSupportDirectory, NSUserDomainMask, YES );
		if ( [paths count] == 0 )
		{
			return "";
		}
		
		NSString *resolvedPath = [paths objectAtIndex:0];
		resolvedPath = [resolvedPath stringByAppendingPathComponent: @"OpenVR"];
		
		if ( ![[NSFileManager defaultManager] createDirectoryAtPath: resolvedPath withIntermediateDirectories:YES attributes:nil error:nil] )
		{
			return "";
		}
		
		sSettingsDir.assign( [resolvedPath UTF8String] );
	}
	return sSettingsDir;
#elif defined( LINUX )

	// As defined by XDG Base Directory Specification 
	// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

	const char *pchHome = getenv("XDG_CONFIG_HOME");
	if ( ( pchHome != NULL) && ( pchHome[0] != '\0' ) )
	{
		return pchHome;
	}

	//
	// XDG_CONFIG_HOME is not defined, use ~/.config instead
	// 
	pchHome = getenv( "HOME" );
	if ( pchHome == NULL )
	{
		return "";
	}

	std::string sUserPath( pchHome );
	sUserPath = Path_Join( sUserPath, ".config" );
	return sUserPath;
#else
	#warning "Unsupported platform"
#endif
}


// ---------------------------------------------------------------------------
// Purpose: Constructor
// ---------------------------------------------------------------------------
CVRPathRegistry_Public::CVRPathRegistry_Public()
{

}

// ---------------------------------------------------------------------------
// Purpose: Computes the registry filename
// ---------------------------------------------------------------------------
std::string CVRPathRegistry_Public::GetOpenVRConfigPath()
{
	std::string sConfigPath = GetAppSettingsPath();
	if( sConfigPath.empty() )
		return "";

#if defined( _WIN32 ) || defined( LINUX )
	sConfigPath = Path_Join( sConfigPath, "openvr" );
#elif defined ( OSX ) 
	sConfigPath = Path_Join( sConfigPath, ".openvr" );
#else
	#warning "Unsupported platform"
#endif
	sConfigPath = Path_FixSlashes( sConfigPath );
	return sConfigPath;
}



//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
std::string CVRPathRegistry_Public::GetVRPathRegistryFilename()
{
	std::string sOverridePath = GetEnvironmentVariable( "VR_PATHREG_OVERRIDE" );
	if ( !sOverridePath.empty() )
		return sOverridePath;

	std::string sPath = GetOpenVRConfigPath();
	if ( sPath.empty() )
		return "";

#if defined( _WIN32 )
	sPath = Path_Join( sPath, "openvrpaths.vrpath" );
#elif defined ( POSIX ) 
	sPath = Path_Join( sPath, "openvrpaths.vrpath" );
#else
	#error "Unsupported platform"
#endif
	sPath = Path_FixSlashes( sPath );
	return sPath;
}


// ---------------------------------------------------------------------------
// Purpose: Converts JSON to a history array
// ---------------------------------------------------------------------------
static void ParseStringListFromJson( std::vector< std::string > *pvecHistory, const Json::Value & root, const char *pchArrayName )
{
	if( !root.isMember( pchArrayName ) )
		return;

	const Json::Value & arrayNode = root[ pchArrayName ];
	if( !arrayNode )
	{
		VRLog( "VR Path Registry node %s is not an array\n", pchArrayName );
		return;
	}

	pvecHistory->clear();
	pvecHistory->reserve( arrayNode.size() );
	for( uint32_t unIndex = 0; unIndex < arrayNode.size(); unIndex++ )
	{
		std::string sPath( arrayNode[ unIndex ].asString() );
		pvecHistory->push_back( sPath );
	}
}


// ---------------------------------------------------------------------------
// Purpose: Converts a history array to JSON
// ---------------------------------------------------------------------------
static void StringListToJson( const std::vector< std::string > & vecHistory, Json::Value & root, const char *pchArrayName )
{
	Json::Value & arrayNode = root[ pchArrayName ];
	for( auto i = vecHistory.begin(); i != vecHistory.end(); i++ )
	{
		arrayNode.append( *i );
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CVRPathRegistry_Public::ToJsonString( std::string &sJsonString )
{
	std::string sRegPath = GetVRPathRegistryFilename();
	if( sRegPath.empty() )
		return false;
	
	std::string sRegistryContents = Path_ReadTextFile( sRegPath );
	if( sRegistryContents.empty() )
		return false;

	sJsonString = sRegistryContents;

	return true;
}


// ---------------------------------------------------------------------------
// Purpose: Loads the config file from its well known location
// ---------------------------------------------------------------------------
bool CVRPathRegistry_Public::BLoadFromFile( std::string *psLoadError )
{
	std::string sRegPath = GetVRPathRegistryFilename();
	if( sRegPath.empty() )
	{
		if ( psLoadError )
		{
			*psLoadError = "Unable to determine VR Path Registry filename";
		}
		return false;
	}

	std::string sRegistryContents = Path_ReadTextFile( sRegPath );
	if( sRegistryContents.empty() )
	{
		if ( psLoadError )
		{
			*psLoadError = "Unable to read VR Path Registry from " + sRegPath;
		}
		return false;
	}

	Json::Value root;
	Json::CharReaderBuilder builder;
	std::istringstream istream( sRegistryContents );
	std::string sErrors;

	try {
		if ( !parseFromStream( builder, istream, &root, &sErrors ) )
		{
			if ( psLoadError )
			{
				*psLoadError = "Unable to parse " + sRegPath + ": " + sErrors;
			}
			return false;
		}

		ParseStringListFromJson( &m_vecRuntimePath, root, "runtime" );
		ParseStringListFromJson( &m_vecConfigPath, root, "config" );
		ParseStringListFromJson( &m_vecLogPath, root, "log" );
		if ( root.isMember( "external_drivers" ) && root["external_drivers"].isArray() )
		{
			ParseStringListFromJson( &m_vecExternalDrivers, root, "external_drivers" );
		}
	}
	catch ( ... )
	{
		if ( psLoadError )
		{
			*psLoadError = "Unable to parse " + sRegPath + ": exception thrown in JSON library";
		}
		return false;
	}

	return true;
}


// ---------------------------------------------------------------------------
// Purpose: Saves the config file to its well known location
// ---------------------------------------------------------------------------
bool CVRPathRegistry_Public::BSaveToFile() const
{
	std::string sRegPath = GetVRPathRegistryFilename();
	if( sRegPath.empty() )
		return false;
	
	Json::Value root;
	
	root[ "version" ] = 1;
	root[ "jsonid" ] = "vrpathreg";

	StringListToJson( m_vecRuntimePath, root, "runtime" );
	StringListToJson( m_vecConfigPath, root, "config" );
	StringListToJson( m_vecLogPath, root, "log" );
	StringListToJson( m_vecExternalDrivers, root, "external_drivers" );

	Json::StreamWriterBuilder builder;
	std::string sRegistryContents = Json::writeString( builder, root );

	// make sure the directory we're writing into actually exists
	std::string sRegDirectory = Path_StripFilename( sRegPath );
	if( !BCreateDirectoryRecursive( sRegDirectory.c_str() ) )
	{
		VRLog( "Unable to create path registry directory %s\n", sRegDirectory.c_str() );
		return false;
	}

	if( !Path_WriteStringToTextFile( sRegPath, sRegistryContents.c_str() ) )
	{
		VRLog( "Unable to write VR path registry to %s\n", sRegPath.c_str() );
		return false;
	}

	return true;
}


// ---------------------------------------------------------------------------
// Purpose: Returns the current runtime path or NULL if no path is configured.
// ---------------------------------------------------------------------------
std::string CVRPathRegistry_Public::GetRuntimePath() const
{
	if( m_vecRuntimePath.empty() )
		return "";
	else
		return m_vecRuntimePath.front().c_str();
}


// ---------------------------------------------------------------------------
// Purpose: Returns the current config path or NULL if no path is configured.
// ---------------------------------------------------------------------------
std::string CVRPathRegistry_Public::GetConfigPath() const
{
	if( m_vecConfigPath.empty() )
		return "";
	else
		return m_vecConfigPath.front().c_str();
}


// ---------------------------------------------------------------------------
// Purpose: Returns the current log path or NULL if no path is configured.
// ---------------------------------------------------------------------------
std::string CVRPathRegistry_Public::GetLogPath() const
{
	if( m_vecLogPath.empty() )
		return "";
	else
		return m_vecLogPath.front().c_str();
}



// ---------------------------------------------------------------------------
// Purpose: Returns paths using the path registry and the provided override 
//			values. Pass NULL for any paths you don't care about.
// ---------------------------------------------------------------------------
bool CVRPathRegistry_Public::GetPaths( std::string *psRuntimePath, std::string *psConfigPath, std::string *psLogPath, const char *pchConfigPathOverride, const char *pchLogPathOverride, std::vector<std::string> *pvecExternalDrivers )
{
	std::string sLoadError;
	CVRPathRegistry_Public pathReg;
	bool bLoadedRegistry = pathReg.BLoadFromFile( &sLoadError );
	int nCountEnvironmentVariables = 0;
	int nRequestedPaths = 0;

	if( psRuntimePath )
	{
		nRequestedPaths++;
		if ( GetEnvironmentVariable( k_pchRuntimeOverrideVar ).length() != 0 )
		{
			*psRuntimePath = GetEnvironmentVariable( k_pchRuntimeOverrideVar );
			nCountEnvironmentVariables++;
		}
		else if( !pathReg.GetRuntimePath().empty() )
		{
			*psRuntimePath = pathReg.GetRuntimePath();
		}
		else
		{
			*psRuntimePath = "";
		}
	}

	if( psConfigPath )
	{
		nRequestedPaths++;
		if ( GetEnvironmentVariable( k_pchConfigOverrideVar ).length() != 0 )
		{
			*psConfigPath = GetEnvironmentVariable( k_pchConfigOverrideVar );
			nCountEnvironmentVariables++;
		}
		else if( pchConfigPathOverride )
		{
			*psConfigPath = pchConfigPathOverride;
		}
		else if( !pathReg.GetConfigPath().empty() )
		{
			*psConfigPath = pathReg.GetConfigPath();
		}
		else
		{
			*psConfigPath = "";
		}
	}

	if( psLogPath )
	{
		nRequestedPaths++;
		if ( GetEnvironmentVariable( k_pchLogOverrideVar ).length() != 0 )
		{
			*psLogPath = GetEnvironmentVariable( k_pchLogOverrideVar );
			nCountEnvironmentVariables++;
		}
		else if( pchLogPathOverride )
		{
			*psLogPath = pchLogPathOverride;
		}
		else if( !pathReg.GetLogPath().empty() )
		{
			*psLogPath = pathReg.GetLogPath();
		}
		else
		{
			*psLogPath = "";
		}
	}

	if ( pvecExternalDrivers )
	{
		*pvecExternalDrivers = pathReg.m_vecExternalDrivers;
	}

	if ( nCountEnvironmentVariables == nRequestedPaths )
	{
		// all three environment variables were set, so we don't need the physical file
		return true;
	}
	else if( !bLoadedRegistry )
	{
		VRLog( "%s\n", sLoadError.c_str() );
	}

	return bLoadedRegistry;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
uint32_t CVRPathRegistry_Public::GetSteamAppId()
{
#if !defined( REL_BRANCH_ONLY )
	uint32_t nSteamAppId = k_unSteamVRMainAppId;
#else
	uint32_t nSteamAppId = k_unSteamVRAppId;
#endif

	return nSteamAppId;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CVRPathRegistry_Public::IsSteamVRMain()
{
#if defined( REL_BRANCH_ONLY )
	return false;
#else
	return true;
#endif
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
uint32_t CVRPathRegistry_Public::InitSteamAppId()
{
	uint32_t nSteamAppId = CVRPathRegistry_Public::GetSteamAppId();

	// Forcefully setting to what it should be before SteamAPI_Init() since SteamVR is more often
	// started as child processes of the game app and otherwise Steam then considers us as the
	// wrong app id.
	SetEnvironmentVariable( L"SteamAppId", std::to_wstring( nSteamAppId ).c_str() );
	SetEnvironmentVariable( L"SteamGameId", std::to_wstring( nSteamAppId ).c_str() );

	return nSteamAppId;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CVRPathRegistry_Public::IsChildOfVRServer()
{
	std::string app_key = GetEnvironmentVariable( "STEAMVR_APPKEY" );
	return app_key == "openvr.component.vrserver";
}
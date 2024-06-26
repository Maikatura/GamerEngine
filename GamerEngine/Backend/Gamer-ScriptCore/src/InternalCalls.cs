using System;
using System.Runtime.CompilerServices;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace GamerEngine
{
	public static class InternalCalls
	{
		#region Entity
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static ulong Entity_FindEntityByName(string name);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_GetName(ulong entityID);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasChildren(ulong entityID);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int Entity_GetChildCount(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_GetChild(ulong entityID, int childIndex);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static object GetScriptInstance(ulong entityID);
		#endregion

		#region TransformComponent
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(ulong entityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(ulong entityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetForward(ulong entityID, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRight(ulong entityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetUp(ulong entityID, out Vector3 translation);

 
        #endregion

        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_Keyboard_IsKeyDown(KeyCode keycode);
        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_Keyboard_IsKeyUp(KeyCode keycode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_Keyboard_IsKeyPressed(KeyCode keycode);
        
        
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_Mouse_IsKeyDown(MouseKeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_Mouse_IsKeyUp(MouseKeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_Mouse_IsKeyPressedThisFrame(MouseKeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_Mouse_GetDelta(out Vector2 aMouseDelta);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Input_Mouse_GetScrollWheel();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_Mouse_LockMouse(MouseLock aLockMouseState);

        #endregion
    }
}
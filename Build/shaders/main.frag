#version 330 core

out vec4 FragColor;
in vec3 WorldPos;
in vec3 Normal;
in vec4 Color;

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// lights
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 camPos;

void main()
{
  FragColor = Color;
}

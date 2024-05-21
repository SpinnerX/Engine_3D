// // // Renderer2D Circle Shader

// // #type vertex
// // #version 330 core

// // layout(location = 0) in vec3 a_WorldPosition;
// // layout(location = 1) in vec3 a_LocalPosition;
// // layout(location = 2) in vec4 a_Color;
// // layout(location = 3) in float a_Thickness;
// // layout(location = 4) in float a_Fade;
// // layout(location = 5) in float a_EntityID; //! @note KEY-NOTE that it is a float because this shader was developed on M1 mac that does not support "flat" keyword with use of layouts locations....


// // //! @note Use this when on a non-mac platform
// // // layout(std140, bind = 0) uniform camera {
// // //     mat4 u_ViewProjection;
// // // };

// // uniform mat4 u_ViewProjection;

// // struct VertexOutput{
// //     vec3 LocalPosition;
// //     vec4 Color;
// //     float Thickness;
// //     float Fade;
// // };

// // // out vec3 LocalPosition;
// // // out vec4 Color;
// // // out float Thickness;
// // // out float Fade;

// // out VertexOutput Output;
// // out float v_EntityID;
// // //! @note Replace these two lines with the following below
// // // layout (location = 0) out VertexOutput output;
// // // layout (location = 1) out VertexOutput v_EntityID;

// // void main(){
// //     Output.LocalPosition = a_LocalPosition;
// //     Output.Color = a_Color;
// //     Output.Thickness = a_Thickness;
// //     Output.Fade = a_Fade;
// // 	v_EntityID = a_EntityID;

// // 	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
// // }

// // #type fragment
// // #version 330 core

// // layout(location = 0) out vec4 o_Color;
// // layout(location = 1) out float o_EntityID;

// // struct VertexOutput{
// //     vec3 LocalPosition;
// //     vec4 Color;
// //     float Thickness;
// //     float Fade;
// // };

// // // in vec3 LocalPosition;
// // // in vec4 Color;
// // // in float Thickness;
// // // in float Fade;

// // in VertexOutput Output;
// // in float v_EntityID;
// // // layout(location = 0) in VertexOutput Output;
// // // layout(location = 4) in flat int v_EntityID;

// // void main(){
// //     float Distance = 1.0 - length(Output.LocalPosition);
// //     float circle = smoothstep(0.0, Output.Fade, Distance);
// //     circle *= smoothstep(Output.Thickness + Output.Fade, Output.Thickness, Distance);

// //     if(circle == 0.0)
// //         discard;

// //     // Setting Outputted color
// //     o_Color = Output.Color;
// // 	o_Color.a *= circle;

// //     o_EntityID = v_EntityID;
// // }


// #type vertex
// #version 330 core

// layout(location = 0) in vec3 a_WorldPosition;
// layout(location = 1) in vec3 a_LocalPosition;
// layout(location = 2) in vec4 a_Color;
// layout(location = 3) in float a_Thickness;
// layout(location = 4) in float a_Fade;
// layout(location = 5) in float a_EntityID;

// // layout(std140, binding = 0) uniform Camera
// // {
// // 	mat4 u_ViewProjection;
// // };

// uniform mat4 u_ViewProjection;

// struct VertexOutput{
// 	vec3 LocalPosition;
// 	vec4 Color;
// 	float Thickness;
// 	float Fade;
// };

// out VertexOutput Input;
// out float v_EntityID;

// void main(){
// 	Input.LocalPosition = a_LocalPosition;
// 	Input.Color = a_Color;
// 	Input.Thickness = a_Thickness;
// 	Input.Fade = a_Fade;

// 	v_EntityID = a_EntityID;

// 	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
// }

// #type fragment
// #version 330 core

// layout(location = 0) out vec4 o_Color;
// layout(location = 1) out float o_EntityID;

// struct VertexOutput{
// 	vec3 LocalPosition;
// 	vec4 Color;
// 	float Thickness;
// 	float Fade;
// };

// in VertexOutput Input;
// in float v_EntityID;

// void main()
// {
//     // Calculate distance and fill circle with white
//     float distance = 1.0 - length(Input.LocalPosition);
//     float circle = smoothstep(0.0, Input.Fade, distance);
//     circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

// 	if (circle == 0.0)
// 		discard;

//     // Set output color
//     o_Color = Input.Color;
// 	o_Color.a *= circle;

// 	o_EntityID = v_EntityID;
// }
// Renderer2D Quad Shader

#type vertex
#version 330 core

// Input attributes
layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

// Uniform block
uniform Camera {
    mat4 u_ViewProjection;
};

// Output variables
out vec3 v_LocalPosition;
out vec4 v_Color;
out float v_Thickness;
out float v_Fade;
flat out int v_EntityID;

void main() {
    // Pass attributes to the fragment shader
    v_LocalPosition = a_LocalPosition;
    v_Color = a_Color;
    v_Thickness = a_Thickness;
    v_Fade = a_Fade;
	    v_EntityID = a_EntityID;

    // Transform the position to clip-space
    gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out float o_EntityID;

in vec3 v_LocalPosition;
in vec4 v_Color;
in float v_Thickness;
in float v_Fade;
flat in float v_EntityID;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(v_LocalPosition);
    float circle = smoothstep(0.0, v_Fade, distance);
    circle *= smoothstep(v_Thickness + v_Fade, v_Thickness, distance);
    
    if (circle == 0.0)
        discard;

    // Set output color
    o_Color = v_Color;
    o_Color.a *= circle;
    o_EntityID = v_EntityID;
}
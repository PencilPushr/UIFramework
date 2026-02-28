#pragma once

//
// Note: This library is HEAVILY inspired by "Dear ImGui", as a result of the shortcomings in v1.4 of the library
// 
// For lack of a better explanation, for all intents and purposes this is a worse implementation.
// Use only for learning purposes.
//

/*
	Structure of ImUI:
	1) Includes
	2) Typedefs
	3) Structures
		4) Utility functions - supporting section for (3) Structures
	unk?) Utility functions for Main
	5) Main functionality
	6) Widget implementation

*/

//
// 1) Includes
//

#include <stdint.h>
#include <string.h>
#include <math.h>

// This is where you would pick the renderer
// #include "renderer.h"

/* 
I think it would best make sense to use CRTP or pick the renderer at compile time.
 Picking and switching a renderer at runtime does not seem like a good idea.
 */


//
// 2) Typedefs
//

typedef unsigned int ImID;			// A unique ID used to identify UI elements
typedef unsigned int ImU32;			// 32-bit unsigned for colors
typedef int ImUIWindowFlags;		// Window behavior flags
typedef int ImUIChildFlags;			// Child window flags



#ifdef __cplusplus
#include <cstddef>

extern "C" {
#endif

/**
* Reference: imgui.h lines 254 - 280
*
* @brief
*
*/
	struct ImVec2
	{
		float x, y;

		// Ctor
		constexpr ImVec2() : x( 0.0f ), y( 0.0f ) {};
		constexpr ImVec2( float _x, float _y ) : x( _x ), y( _y ) {};

		// Unsure if array access is necessary

		// Arithemetic operations
		ImVec2 operator+( const ImVec2& rhs ) const { return ImVec2( x + rhs.x, y + rhs.y ); }
		ImVec2 operator-( const ImVec2& rhs ) const { return ImVec2( x - rhs.y, y - rhs.y ); }
		ImVec2 operator*( const ImVec2& rhs ) const { return ImVec2( x * rhs.x, y * rhs.y ); }
		ImVec2 operator*( float rhs )		  const { return ImVec2( x * rhs, y * rhs ); }
		ImVec2 operator/( float rhs )		  const { return ImVec2( x / rhs, y / rhs ); }

		// Compound assignment
		ImVec2& operator+=( const ImVec2& rhs ) { x += rhs.x; y += rhs.y; return *this; }
		ImVec2& operator-=( const ImVec2& rhs ) { x -= rhs.x; y -= rhs.y; return *this; }
		ImVec2& operator*=( const ImVec2& rhs ) { x *= rhs.x; y *= rhs.y; return *this; }
		ImVec2& operator/=( const ImVec2& rhs ) { x /= rhs.x; y /= rhs.y; return *this; }

		bool operator==( const ImVec2& rhs ) { return x == rhs.x && y == rhs.y; }
		bool operator!=( const ImVec2& rhs ) { return x != rhs.x || y != rhs.y; }
	};

// ImVec4 is not implemented as we have the COLOR structure
// See renderer.h for this include
// Change this to IF NOT DEFINED when we fix the includes
#ifdef SR_API_DEFINED
    typedef struct _COLOUR
    {
        union
        {
            FLOAT RGBA[ 4 ];

            struct
            {
                FLOAT Red;
                FLOAT Green;
                FLOAT Blue;
                FLOAT Alpha;
            };

            struct
            {
                FLOAT red;
                FLOAT green;
                FLOAT blue;
                FLOAT alpha;
            };

            struct
            {
                FLOAT RED;
                FLOAT GREEN;
                FLOAT BLUE;
                FLOAT ALPHA;
            };

            struct
            {
                FLOAT R;
                FLOAT G;
                FLOAT B;
                FLOAT A;
            };

            struct
            {
                FLOAT r;
                FLOAT g;
                FLOAT b;
                FLOAT a;
            };

            struct
            {
                FLOAT Hue;
                union
                {
                    FLOAT Saturation;
                    FLOAT Sat;
                };
                FLOAT Value;
            };

            struct
            {
                FLOAT hue;
                union
                {
                    FLOAT saturation;
                    FLOAT sat;
                };
                FLOAT value;
            };

            struct
            {
                FLOAT HUE;
                union
                {
                    FLOAT SATURATION;
                    FLOAT SAT;
                };
                FLOAT VALUE;
            };

            struct
            {
                FLOAT H;
                FLOAT S;
                FLOAT V;
            };

            struct
            {
                FLOAT h;
                FLOAT s;
                FLOAT v;
            };
        };

    } COLOUR, * PCOLOUR, _COLOR, COLOR, * PCOLOR;

#endif


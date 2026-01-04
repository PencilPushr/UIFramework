#pragma once

//
// Note: This library is HEAVILY inspired by ImGui, as a result of the shortcomings in v1 of the library
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

#include "renderer.h"

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
        float x , y;

        // Ctor
        constexpr ImVec2() : x( 0.0f ) , y( 0.0f ) {};
        constexpr ImVec2( float _x , float _y ) : x( _x ) , y( _y ) {};

        // Unsure if array access is necessary

        // Arithemetic operations
        ImVec2 operator+( const ImVec2 & rhs ) const { return ImVec2( x + rhs.x , y + rhs.y ); }
        ImVec2 operator-( const ImVec2 & rhs ) const { return ImVec2( x - rhs.y , y - rhs.y ); }
        ImVec2 operator*( const ImVec2 & rhs ) const { return ImVec2( x * rhs.x , y * rhs.y ); }
        ImVec2 operator*( float rhs )		  const { return ImVec2( x * rhs , y * rhs ); }
        ImVec2 operator/( float rhs )		  const { return ImVec2( x / rhs , y / rhs ); }

        // Compound assignment
        ImVec2 & operator+=( const ImVec2 & rhs ) { x += rhs.x; y += rhs.y; return *this; }
        ImVec2 & operator-=( const ImVec2 & rhs ) { x -= rhs.x; y -= rhs.y; return *this; }
        ImVec2 & operator*=( const ImVec2 & rhs ) { x *= rhs.x; y *= rhs.y; return *this; }
        ImVec2 & operator/=( const ImVec2 & rhs ) { x /= rhs.x; y /= rhs.y; return *this; }

        bool operator==( const ImVec2 & rhs ) { return x == rhs.x && y == rhs.y; }
        bool operator!=( const ImVec2 & rhs ) { return x != rhs.x || y != rhs.y; }
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

    } COLOUR , * PCOLOUR , _COLOR , COLOR , * PCOLOR;

#endif

    //
    // 4) Utility functions
    // 

    /**
    * Reference: imgui.h lines 270 - 290
    *
    * ImVec2 Helper Functions
    *
    */
    static ImVec2 ImUI_ZeroVecFactory() { return ImVec2( 0.0f , 0.0f ); }
    static inline float  ImVec2_LengthSqr( ImVec2 v ) { return v.x * v.x + v.y * v.y; }

    static inline ImVec2 ImMin( ImVec2 lhs , ImVec2 rhs )
    {
        return { lhs.x < rhs.x ? lhs.x : rhs.x , lhs.y < rhs.y ? lhs.y : rhs.y };
    }

    static inline ImVec2 ImMax( ImVec2 lhs , ImVec2 rhs )
    {
        return { lhs.x > rhs.x ? lhs.x : rhs.x , lhs.y > rhs.y ? lhs.y : rhs.y };
    }

    static inline ImVec2 ImClamp( ImVec2 value , ImVec2 min , ImVec2 max )
    {
        return
        {
            ( value.x < min.x ) ? min.x : ( value.x > max.x ) ? max.x : value.x ,
            ( value.y < min.y ) ? min.y : ( value.y > max.y ) ? max.y : value.y
        };
    }

    static inline ImVec2 ImLerp( ImVec2 a , ImVec2 b , float t )
    {
        return { a.x + ( b.x - a.x ) * t , a.y + ( b.y - a.y ) * t };
    }

    static inline ImVec2 ImFloor( ImVec2 v )
    {
        return { floorf( v.x ) , floorf( v.y ) };
    }

    static inline float ImDot( ImVec2 a , ImVec2 b )
    {
        return a.x * b.x + a.y * b.y;
    }

    /**
    * Reference: imgui_internal.h lines 475-550
    *
    * ImRect implementation
    */
    struct ImRect
    {
        ImVec2 Min;     // Top-left
        ImVec2 Max;     // Bottom-right (exclusive)

        // Ctors
        constexpr ImRect() : Min( 0 , 0 ) , Max( 0 , 0 ) {}
        constexpr ImRect( const ImVec2 & min , const ImVec2 & max ) : Min( min ) , Max( max ) {}
        constexpr ImRect( float x1 , float y1 , float x2 , float y2 ) : Min( x1 , y1 ) , Max( x2 , y2 ) {}

    };


    //
    // 5) Main functionality
    //
    bool ImUI_Init( void );
    bool ImUI_BeginWindow( const char * pcName , bool * pOpen , ImUIWindowFlags imFlags );

    // 6) Widget implementations
    bool ImUI_Button( const char * pLabel , bool * pPressed );

#ifdef __cplusplus
}
#endif

// Provide template overloads if we are working in C++
#ifdef __cplusplus

template < typename T , std::size_t N >
struct ImVec2_t {}; // declare as empty for now, will shut the compiler up
#endif
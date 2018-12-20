//
//  GemUtils.h
//  HelloCpp
//
//  Created by 毛 靖凯 on 13-9-2.
//
//

#ifndef HelloCpp_GemUtils_h
#define HelloCpp_GemUtils_h

#include <cocos2d.h>
//#include <gfx/vec2.h>
//#include <boost/lexical_cast.hpp>

using namespace cocos2d;

namespace PH
{

#define BOARD_WIDTH 6
#define BOARD_HEIGHT 5

	enum BattleUIOrder
	{
		ORDER_BACKGROUND = -10,    // background
		ORDER_BOARD = 0,           // items in board
		ORDER_ENEMY_STATUS_LABEL = 25, //
		ORDER_HERO_ICON = 50,
		ORDER_ABOVE_ICON = 75,     // right above icons
		ORDER_MASK = 100,          // board mask
		ORDER_FLOAT = 125,         // stuff above mask
		ORDER_STATUS_LABEL = 150,
		ORDER_ORB = 200,
		ORDER_VICTORY = 600,
	};

    const static float skGemPixelWidth = 107;
    const static float skGemPixelHeight = 107;
    
    // grid to world and vice versa
    inline Vec2 g2w(const Vec2& p)
    {
        return Vec2(p.x * skGemPixelWidth,
                   p.y * skGemPixelHeight);
    }
    
    inline Vec2 g2w_center(const Vec2 & p)
    {
        return Vec2(p.x * skGemPixelWidth + skGemPixelWidth/2,
                   p.y * skGemPixelHeight + skGemPixelHeight/2);
    }

	// Random number generators
	inline float randf()
	{
		return (float)rand() * (1.0f / (float)RAND_MAX);
	}

	inline float randf(float max)
	{
		return randf() * max;
	}
	inline float randf(float min, float max)
	{
		return randf(max - min) + min;
	}

	inline Vec2 randomUnitVec2()
	{
		Vec2 v(randf() - 0.5f, randf() - 0.5f);
		//unitize(v);
		
		return v;
	}

	inline Vec2 randomBand(float min, float max)
	{
		Vec2 normal = randomUnitVec2();
		return normal * randf(min, max);
	}
	inline Point ccRandomBand(float min, float max)
	{
		Vec2 val = randomBand(min, max);
		return val;
	}
	
    inline Vec2 w2g(const Vec2& p)
    {
        return Vec2(floorf(p.x / skGemPixelWidth),
                     floorf(p.y / skGemPixelHeight));
    }
	inline bool resourcePresentForLoading(const std::string& filename)
	{
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename.c_str());
		return FileUtils::getInstance()->isFileExist(fullPath);
	}
	/*
	template<class T> inline T norm2(const TVec2<T>& v) { 
		return v * v; 
	}
	template<class T> inline T norm(const TVec2<T>& v) { 
		return sqrt(norm2(v)); 
	}

	template<class T> inline void unitize(TVec2<T>& v)
	{
		T l = norm2(v);
		if (l != 1.0 && l != 0.0)  v /= sqrt(l);
	}
	*/
	inline ccBezierConfig ccbz(Vec2 a, Vec2 b, Vec2 c)
	{
		ccBezierConfig config;
		config.controlPoint_1 = a;
		config.controlPoint_2 = b;
		config.endPosition = c;

		return config;
	}

    inline Vec2 roundToGrid(Vec2& p){  return g2w(w2g(p)); }
    
    struct GemUtils {  // tolua_export
        // tolua_begin
        enum GemColor
        {
            // DO NOT change order of these values.
            // Frst value MUST start from 0.
            // AllColor MUST immediately follow the last gem color.
            // Health must be the last among all actual gem colors.
            Fire = 0,
            Water = 1,
            Wood = 2,
            Dark = 3,
            Light = 4,
            Health = 5,
            AllColor = 6,
            GemColorMax = 7,
        };
        // tolua_end
        
        static inline bool isEqualColorOrAllColor(GemColor col, GemColor tarCol){
            if(col == tarCol || col == GemColor::AllColor) return true;
            return false;
        }

        
        static std::string getNameWithPatternAndColor(std::string patter,
                                                      GemColor color);
        static const char* res(GemColor c);  // tolua_export
        static const char* frame(GemColor c);  // tolua_export
        static const char* name(GemColor c); // tolua_export
        static std::string nameCN(GemColor c); // tolua_export
        static const char* numberFont(GemColor c); //tolua_export
        static const char* resistanceFont(GemColor c);
        static const char* healthBox(GemColor c); // tolua_export
        static const char* healthFill(GemColor c); // tolua_export
        
        static const char* healthBox(GemColor c, bool isLong);
        static const char* healthFill(GemColor c, bool isLong);
        
        static const char* attackSound(GemColor c);
        static GemUtils::GemColor strToColor(std::string str);
        static const char* colorToStr(GemColor c);
        static CCSpriteFrame* energy(GemColor c, int val);
        static const char* attackAnim(GemColor c);
        static const char* stageColorRes(GemColor c);
        static ccColor3B colorToCCColor(GemColor c);

		static cocos2d::Sprite * GetSprite(const std::string& filename);

        static inline std::string i(GemColor c){

			std::stringstream stream;
			stream << c;
            return stream.str();
        }
        
        static GemUtils::GemColor weak(GemColor l);
        static inline bool strongvsweak(GemColor l, GemColor r){
            return weak(l) == r;
        }
        static inline int compare(GemUtils::GemColor l, GemUtils::GemColor r) {
            // order of comparison should not change, consider Fire vs Light
            if(strongvsweak(l, r)) return 1;
            else if(strongvsweak(r, l)) return -1;
            else
                return 0;
        }
        
        static inline float modifier(GemUtils::GemColor l, GemUtils::GemColor r) {
            int cmp = compare(l,r);
            if(cmp > 0) return 2.0f;
            else if(cmp < 0) return 0.5f;
            else return 1.0f;
        }
    }; //tolua_export
    
}

#endif

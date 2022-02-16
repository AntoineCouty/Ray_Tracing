#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "base_light.hpp"
namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:
		virtual ~QuadLight() = default;

		QuadLight( const Vec3f & p_color,
				   float		 p_power,
				   const Vec3f & p_position,
				   const Vec3f & p_u,
				   const Vec3f & p_v )
			: BaseLight( p_color, p_power ), _position( p_position ), _u( p_u ), _v( p_v )
		{
			_normal = glm::normalize(glm::cross( _v, _u ));
			_area	= _u.length() * _v.length();
			_isSurface = true;
		}

		inline const Vec3f & getPosition() const { return _position; }

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  private:
		Vec3f _position = VEC3F_ZERO;
		Vec3f _u		= VEC3F_ZERO;
		Vec3f _v		= VEC3F_ZERO;
		Vec3f _normal	= VEC3F_ZERO;
		float _area;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_QUAD_LIGHT__
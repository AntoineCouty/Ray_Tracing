#ifndef __RT_ISICG_CIRCLE_LIGHT__
#define __RT_ISICG_CIRCLE_LIGHT__

#include "base_light.hpp"
namespace RT_ISICG
{
	class CircleLight : public BaseLight
	{
	  public:
		virtual ~CircleLight() = default;

		CircleLight( const Vec3f & p_color,
				   float		 p_power,
				   const Vec3f & p_position,
				   const Vec3f & p_u,
				   const Vec3f & p_v,
				   float p_rayon)
			: BaseLight( p_color, p_power ), _position( p_position ), _u( p_u ), _v( p_v ), _rayon(p_rayon)
		{
			_normal	   = glm::normalize( glm::cross( _v, _u ) );
			_area	   = glm::pi<float>() * _rayon * _rayon;
			_isSurface = true;
		}

		inline const Vec3f & getPosition() const { return _position; }

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  private:
		Vec3f _position = VEC3F_ZERO;
		Vec3f _u		= VEC3F_ZERO;
		Vec3f _v		= VEC3F_ZERO;
		Vec3f _normal	= VEC3F_ZERO;
		float _rayon	= 1.f;
		float _area;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_CIRCLE_LIGHT__
#ifndef __RT_ISICG_SPOT_LIGHT__
#define __RT_ISICG_SPOT_LIGHT__

#include "base_light.hpp"
namespace RT_ISICG
{
	class SpotLight : public BaseLight
	{
	  public:
		virtual ~SpotLight() = default;

		SpotLight( const Vec3f & p_color,
					 float		   p_power,
					 const Vec3f & p_position,
					 const Vec3f & p_direction,
					 float		   p_rayon,float p_angle )
			: BaseLight( p_color, p_power ), _position( p_position ), _direction( p_direction ),
			  _rayon( p_rayon ),
			  _angle( p_angle )
		{
			//_normal	   = glm::normalize( glm::cross( _u, _v ) );
			_area	   = glm::pi<float>() * _rayon * _rayon;
			_isSurface = true;
		}

		inline const Vec3f & getPosition() const { return _position; }

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  private:
		Vec3f _position = VEC3F_ZERO;
		Vec3f _direction = VEC3F_ZERO;
		Vec3f _normal	= VEC3F_ZERO;
		float _rayon	= 1.f;
		float _area;
		float _angle; // angle in degress
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_SPOT_LIGHT__
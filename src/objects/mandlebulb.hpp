#ifndef __RT_ISICG_MANDLEBULB__
#define __RT_ISICG_MANDLEBULB__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class Mandlebulb : public ImplicitSurface
	{
	  public:
		Mandlebulb()			 = delete;
		virtual ~Mandlebulb() = default;

		Mandlebulb( const std::string & p_name, Vec3f p_center )
			: ImplicitSurface( p_name ), _center( p_center )
		{
		}

	  private:
		// Signed Distance Function
		float _sdf( const Vec3f & p_point ) const
		{
			Vec3f w = p_point - _center;
			float m = dot( w, w );


			float dz   = 1.0;

			for ( int i = 0; i < 4; i++ )
			{

				// dz = 8*z^7*dz
				dz = 8.0 * glm::pow( m, 3.5 ) * dz + 1.0;

				// z = z^8+z
				float r = glm::length( w );
				float b = 8.0 * glm::acos( w.y / r );
				float a = 8.0 * glm::atan( w.x, w.z );
				w		= p_point - _center
					+ glm::pow( r, 8.f ) * Vec3f( sin( b ) * sin( a ), cos( b ), sin( b ) * cos( a ) );


				m = dot( w, w );
				if ( m > 256.0 ) break;
			}

			// distance estimation (through the Hubbard-Douady potential)
			float sdf = 0.25 * glm::log( m ) * glm::sqrt( m ) / dz;

			return glm::min( sdf, 1.f );
		}

	  private:
		Vec3f _center = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MANDLEBULB__

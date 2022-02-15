#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt,
										  const Vec3f & p_up,
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		_w = glm::normalize( p_position - p_lookAt );
		_u = glm::normalize( glm::cross( p_up, _w ) ); 
		_v = glm::normalize( glm::cross( _w, _u ) );

		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float h	   = glm::tan( glm::radians( _fovy * 0.5 ) ) * _focalDistance * 2; 

		_viewportV = _v * h;
		_viewportU = _u * h * _aspectRatio;

		_viewportTopLeftCorner = _position - _viewportU * Vec3f( 0.5 ) + _viewportV * Vec3f( 0.5 ) - Vec3f( 1.0, 1.0, _focalDistance ) * _w;
	}

} // namespace RT_ISICG

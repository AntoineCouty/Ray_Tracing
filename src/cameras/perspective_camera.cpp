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
		std::cout << _u.x << " " << _u.y << " " << _u.z << std::endl;
		std::cout << _v.x << " " << _v.y << " " << _v.z << std::endl;
		std::cout << _w.x << " " << _w.y << " " << _w.z << std::endl;
		/// TODO ! _u ? _v ? _w 

		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float h	   = glm::tan( glm::radians( _fovy * 0.5 ) ) * _focalDistance * 2; 

		_viewportV = _v * h;
		_viewportU = _u * h * _aspectRatio;

		_viewportTopLeftCorner = _position - _viewportU * Vec3f( 0.5 ) + _viewportV * Vec3f( 0.5 ) - Vec3f(1.0, 1.0, _focalDistance)*_w;
		/// TODO ! _viewportTopLeftCorner ?	_viewportU ? _viewportV ?
	}

} // namespace RT_ISICG

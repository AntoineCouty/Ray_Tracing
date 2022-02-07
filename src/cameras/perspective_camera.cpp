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
		/// TODO ! _u ? _v ? _w ?

		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float h	   = glm::tan( glm::radians( _fovy / 2 ) ) * _focalDistance * 2;
		_viewportV = Vec3f( 0.0, h, 0.0 );

		_viewportU = Vec3f( _aspectRatio, 0.0, 0.0 );

		_viewportTopLeftCorner = _position - _viewportU - _viewportV;
		_viewportTopLeftCorner /= 2;
		_viewportTopLeftCorner.z = _position.z + _focalDistance;
		/// TODO ! _viewportTopLeftCorner ?	_viewportU ? _viewportV ?
	}

} // namespace RT_ISICG

#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lighting_integrator.hpp"
#include "integrators/whitted_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::RAY_CAST: 
			_integrator = new RayCastIntegrator(); 
			break;
		case IntegratorType::DIRECT_LIGHT: 
			_integrator = new DirectLightingIntegrator(); 
			break;
		case IntegratorType::WHITTED: 
			_integrator = new WhittedIntegrator(); 
			break;
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();
		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();
		#pragma omp parallel for schedule( dynamic )
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{	
				Vec3f color = Vec3f( 0.f );
				float ran_x;
				float ran_y;
				for ( int nb = 0; nb < _nbPixelSamples; nb++ ) {
					if ( nb == 0 ) { 
						ran_x = 0.5f;
						ran_y = 0.5f;
					}
					else{
						ran_x		= randomFloat();
						ran_y = randomFloat();
					}
					Ray	  ray	= p_camera->generateRay( float( i + ran_x ) / ( width - 1 ),float( j + ran_y ) / ( height - 1 ) );
					color += _integrator->Li( p_scene, ray, 0.f, 1000000.f );
				}
				p_texture.setPixel(i, j, glm::clamp(color/Vec3f(_nbPixelSamples), 0.f, 1.f));
			}
			progressBar.next();
		}
		
		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG

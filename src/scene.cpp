#include "scene.hpp"
#include "materials/color_material.hpp"
#include "materials/lambert_material.hpp"
#include "materials/plastic_material.hpp"
#include "materials/micro_face_material.hpp"
#include "materials/matte_material.hpp"
#include "materials/mirror_material.hpp"
#include "objects/sphere.hpp"
#include "objects/plane.hpp"
#include "lights/point_light.hpp"
#include "lights/quad_light.hpp"
#include "lights/circle_light.hpp"

#define SCENE_1
//#define SCENE_2
namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::init()
	{
#ifdef SCENE_1
		// Add objects.
		 _addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		//_addMaterial( new MicroFaceMaterial( "Grey", Vec3f( 1.f, 0.85f, 0.57f ), 0.3f, 0.5f ) );
		_addMaterial( new MatteMaterial( "Grey", GREY, 0.6f ) );
		//_addMaterial(new PlasticMaterial( "Grey", GREY, 128.f ));
		_addMaterial( new LambertMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Grey", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		_addLight(new PointLight( WHITE, 60, Vec3f( 0.f, 0.f, -2.f ) ) );
		//_addLight( new QuadLight( WHITE, 60, Vec3f( 1.f, 10.f, -2.f ), Vec3f(2.f, 0.f, 0.f), Vec3f(0.f, 0.f, 2.f ) ) );
		//_addLight( new QuadLight( WHITE, 60, Vec3f( -4.f, 1.f, -2.f ), Vec3f( 2.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 2.f ) ) );
#endif
#ifdef SCENE_2
		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new MirrorMaterial( " Mirror " ) );
		_addMaterial( new MicroFaceMaterial( " WhiteMatte ", WHITE, 0.6f, 0.5f ) );
		_addMaterial( new MicroFaceMaterial( " RedMatte ", RED, 0.6f, 0.5f ) );
		_addMaterial( new MicroFaceMaterial( " GreenMatte ", GREEN, 0.6f, 0.5f ) );
		_addMaterial( new MicroFaceMaterial( " BlueMatte ", BLUE, 0.6f, 0.5f ) );
		_addMaterial( new MicroFaceMaterial( " GreyMatte ", GREY, 0.6f, 0.5f ) );
		_addMaterial( new MicroFaceMaterial( " MagentaMatte ", MAGENTA, 0.6f, 0.5f ) );
		// ================================================================
		// Add objects .
		// ================================================================
		// Spheres .
		_addObject( new Sphere( " Sphere1 ", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		 _attachMaterialToObject( " Mirror ", " Sphere1 " );
		_addObject( new Sphere( " Sphere2 ", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( " Mirror ", " Sphere2 " );
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( " GreyMatte ", " PlaneGround " );
		_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( " RedMatte ", " PlaneLeft " );
		_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( " GreenMatte ", " PlaneCeiling " );
		_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		 _attachMaterialToObject( " BlueMatte ", " PlaneRight " );
		_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( " Mirror ", " PlaneFront " );
		// ================================================================
		// Add lights .
		// ================================================================
		_addLight( new PointLight( WHITE, 100.f, Vec3f( 0.f, 5.f, 0.f ) ) );
		//_addLight(new QuadLight( WHITE, 40.f, Vec3f( 1.f, 5.f, -2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ) ) );
#endif
	}
	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const { 
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) )
			{
				return true;
			}
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG

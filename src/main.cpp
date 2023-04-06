#include "transform.h"
#include "sphere.h"
#include "camera.h"
#include <fstream>
#include "film.h"
#include "primitive.h"
#include <memory>
#include "integrator.h"
#include "sampler.h"
#include <iostream>
#include "point.h"
#include "normal.h"
#include "triangle.h"
#include "material.h"
#include "importobj.h"
#include "bvh.h"
#include "disney.h"
#include "parameters.h"
#include "useful.h"




int main(){

   

    PrimitiveList scene;

    Transform SpherePos = Translate(Vector3f(0, -1, -2));
    Transform SpherePosInv = Inv(SpherePos);
    
    Transform SpherePos2 = Translate(Vector3f(0, -100, -3));
    Transform SpherePosInv2 = Inv(SpherePos2);
    
    const Point2i resolution{512, 512};

    std::shared_ptr<Material> floor = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.5, 0.5, 0.5), 0.5, 1.5, 0., 0.));
    std::shared_ptr<Material> ball = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.9, 0., 0.), 0.001, 1.6, 0., 1.));


    std::shared_ptr<Shape> sphere_shape = std::make_shared<Sphere>(&SpherePos, &SpherePosInv, 1);
    std::shared_ptr<Primitive> sphere_prim = std::make_shared<GeoPrimitive>(sphere_shape, ball);

    std::shared_ptr<Shape> sphere2_shape = std::make_shared<Sphere>(&SpherePos2, &SpherePosInv2, 99.0);
    std::shared_ptr<Primitive> sphere2_prim = std::make_shared<GeoPrimitive>(sphere2_shape, floor);

    std::unique_ptr<Point3f[]> verts;
    std::unique_ptr<int[]> vertexIndices;
    int nVertices;
    int nTriangles;



    importOBJ::loadOBJ("stuff/wall.obj", verts, vertexIndices, &nVertices, &nTriangles);
    
    std::vector<std::shared_ptr<Shape>> triangles = CreateTriangleMesh(&SpherePos, &SpherePosInv, 
            nTriangles, vertexIndices.get(), nVertices, verts.get(), nullptr, nullptr);


    //std::shared_ptr<PrimitiveList> mesh = std::make_shared<PrimitiveList>();
    std::vector<std::shared_ptr<Primitive>> mesh;

    for(int i = 0; i < nTriangles; i++){
        std::shared_ptr<Primitive> tri = std::make_shared<GeoPrimitive>(triangles[i], ball);
        // mesh->addPrim(tri);
        mesh.push_back(tri);
    }

    std::shared_ptr<Primitive> BVHmesh = BVHnode::createBVH(mesh);
    scene.addPrim(BVHmesh);


    // scene.addPrim(sphere_prim);

    scene.addPrim(sphere2_prim);
    
    Transform SpherePos3 = Translate(Vector3f(1, 0, -4));
    Transform SpherePosInv3 = Inv(SpherePos3);
    std::shared_ptr<Shape> sphere3_shape = std::make_shared<Sphere>(&SpherePos3, &SpherePosInv3, 1);
    std::shared_ptr<Material> ball2 = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(1, 0, 0), 1, 1.6, 0., 0.));
    std::shared_ptr<Primitive> sphere3_prim = std::make_shared<GeoPrimitive>(sphere3_shape, ball2);
    scene.addPrim(sphere3_prim);
    


    std::shared_ptr<Film> film = std::make_shared<Film>();
    
    Transform cameraToWorld = LookAt(Point3f(0, 3, 0), Point3f(0, 0, 4), Vector3f(0, 1, 0));
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&cameraToWorld, resolution, Radians(90.), 4, 0);
    std::shared_ptr<PixelSampler> sampler = std::make_shared<StratifiedSampler>(49);
    std::cout << sampler->samplesPerPixel << std::endl;
    
    DisneyIntegrator pog{film, camera, sampler};
    std::cout << "Rendering... " << std::endl;
    pog.Render(scene);
    


}

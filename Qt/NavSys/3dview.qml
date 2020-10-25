import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick3D

Window {
  id: window
  visible: true
  width: 1280
  height: 720

  // Viewport for 3D content
  View3D {
    id: view

    anchors.fill: parent
    // Scene to view
    Node {
      id: scene

      Light {

        id: directionalLight

      }

      Camera {
        id: camera
        // It's important that your camera is not inside
        // your model so move it back along the z axis
        // The Camera is implicitly facing up the z axis,
        // so we should be looking towards (0, 0, 0)
        z: -600
      }

      Model {
        id: cubeModel
        // #Cube is one of the "built-in" primitive meshes
        // Other Options are:
        // #Cone, #Sphere, #Cylinder, #Rectangle
        source: "#Cube"

        // When using a Model, it is not enough to have a
        // mesh source (ie "#Cube")
        // You also need to define what material to shade
        // the mesh with. A Model can be built up of
        // multiple sub-meshes, so each mesh needs its own
        // material. Materials are defined in an array,
        // and order reflects which mesh to shade

        // All of the default primitive meshes contain one
        // sub-mesh, so you only need 1 material.

        materials: [

          DefaultMaterial {

            // We are using the DefaultMaterial which
            // dynamically generates a shader based on what
            // properties are set. This means you don't
            // need to write any shader code yourself.
            // In this case we just want the cube to have
            // a red diffuse color.
            id: cubeMaterial
            diffuseColor: "red"
          }
        ]
      }
    }
  }
}

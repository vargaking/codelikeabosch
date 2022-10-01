// import three
import * as THREE from "three";
import { OBJLoader } from "three/examples/jsm/loaders/OBJLoader";
import { MTLLoader } from "three/examples/jsm/loaders/MTLLoader";

class SceneObject {
    // colors for the types
    colors: {[index: string]:number} = {
        main: 0x00ff00,
        car: 0xff0000,
        pedestrian: 0x0000ff
    };

    constructor(public id: number, public x: number, public y: number, public type: string) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.type = type;
    }

    getokObject() {
        const planeGeometry = new THREE.PlaneGeometry(0.5, 1);

        // get the color for the type

        const planeMaterial = new THREE.MeshBasicMaterial({ color: this.colors[this.type], side: THREE.DoubleSide });
        const plane = new THREE.Mesh(planeGeometry, planeMaterial);
        plane.position.x = this.x;
        plane.position.z = this.y;
        plane.position.y = 0.1;

        // rotate the plane
        plane.rotation.x = Math.PI / 2;

        return plane;
    }

    getMainObject(scene) {
        const manager = new THREE.LoadingManager();

        const carLoaderObj = new OBJLoader(manager);
        const carLoaderMtl = new MTLLoader(manager);


        manager.onLoad = () => {
            console.log("loaded");
            document.getElementById("loadingScreen").style.display = "none";
        };

        // load the mtl file
        carLoaderMtl.load("./src/models/kocsi_final.mtl", (materials) => {
            materials.preload();

            // load the obj file
            carLoaderObj.setMaterials(materials);
            carLoaderObj.load("./src/models/kocsi_final.obj", (object) => {
                object.position.y = .45;
                console.log(object);
                // add the object to the scene
                scene.add(object);
            });
        });
    }
}

export { SceneObject };

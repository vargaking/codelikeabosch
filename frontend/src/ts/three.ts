import * as THREE from "three";
import { SceneObject } from "./objects";
// import obj and mtl loader
import { OBJLoader } from "three/examples/jsm/loaders/OBJLoader";
import { MTLLoader } from "three/examples/jsm/loaders/MTLLoader";

class Main {
    scene: THREE.Scene;
    private camera: THREE.PerspectiveCamera;
    private renderer: THREE.WebGLRenderer;

    public objects: SceneObject[] = [];
    public prevObjects: SceneObject[] = [];
    public renderObjects: THREE.Mesh[] = [];

    constructor() {
        this.scene = new THREE.Scene();
        //this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

        this.camera = new THREE.OrthographicCamera(window.innerWidth / -200, window.innerWidth / 200, window.innerHeight / 200, window.innerHeight / -200, -500, 1000);
        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setSize(window.innerWidth, window.innerHeight);
        document.body.appendChild(this.renderer.domElement);

        // 2d scene
        // add a plane
        const planeGeometry = new THREE.PlaneGeometry(5, 5);
        const planeMaterial = new THREE.MeshBasicMaterial({ color: 0xababab, side: THREE.DoubleSide });
        const plane = new THREE.Mesh(planeGeometry, planeMaterial);
        //this.scene.add(plane);

        // create a grid helper
        const gridHelper = new THREE.GridHelper(5, 10);

        this.scene.add(gridHelper);

        // create a grid

        // move the camera to a top view
        //this.camera.position.z = 5;
        this.camera.position.y = 5;
        //this.camera.position.x = 5;
        this.camera.lookAt(0, 0, 0);

        /*
        const objLoader = new OBJLoader();
        const mtlLoader = new MTLLoader();

        // load the mtl file
        mtlLoader.load("./models/kocsi.mtl", (materials) => {
            materials.preload();

            // load the obj file
            objLoader.setMaterials(materials);
            objLoader.load("./models/kocsi.mtl", (object) => {
                // add the object to the scene
                this.scene.add(object);
            });
        });
*/
        this.animate();
    }

    updateScene() {
        console.log(this.objects, "objects", this.objects.length);

        // compare the current objects with the previous objects
        // remove the objects that are not in the current objects
        // add the objects that are in the current objects
        // update the position of the objects that are in the current objects

        // remove the objects that are not in the current objects
        this.prevObjects.forEach((prevObject) => {
            // check if the object is in the current objects
            let objectInCurrentObjects = this.objects.find((object) => {
                return object.id === prevObject.id;
            });

            if (!objectInCurrentObjects) {
                // if the object is not in the current objects, remove it from the scene
                this.scene.remove(prevObject.getCarObject());
            }
        });

        // add the objects that are in the current objects
        for (let i = 0; i < this.objects.length; i++) {
            // check if the object is in the previous objects
            let objectInPrevObjects = this.prevObjects.find((prevObject) => {
                return this.objects[i].id === prevObject.id;
            });

            if (!objectInPrevObjects) {
                // if the object is not in the previous objects, add it to the scene
                this.renderObjects.push(this.objects[i].getCarObject());
                this.scene.add(this.renderObjects[i]);
            }
        }

        // update the position of the objects that are in the current objects
        for (let i = 0; i < this.objects.length; i++) {
            // check if the object is in the previous objects
            let objectInPrevObjects = this.prevObjects.find((prevObject) => {
                return this.objects[i].id === prevObject.id;
            });

            if (objectInPrevObjects) {
                // if the object is in the previous objects, update the position
                this.renderObjects[i].position.x = this.objects[i].x;
                this.renderObjects[i].position.z = this.objects[i].y;
            }
        }

        // set the previous objects to the current objects
        this.prevObjects = this.objects;

        console.log(this.objects);
    }

    animate() {
        requestAnimationFrame(this.animate.bind(this));
        this.renderer.render(this.scene, this.camera);
    }
}

export { Main };

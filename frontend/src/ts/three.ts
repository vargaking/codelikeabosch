import * as THREE from "three";
import { SceneObject } from "./objects";
// import obj and mtl loader
import { OBJLoader } from "three/examples/jsm/loaders/OBJLoader";
import { MTLLoader } from "three/examples/jsm/loaders/MTLLoader";

import { OrbitControls } from "three/examples/jsm/controls/OrbitControls.js";

import carMtl from "./src/models/f.mtl";

class Main {
    scene: THREE.Scene;
    private camera: THREE.PerspectiveCamera;
    private renderer: THREE.WebGLRenderer;

    public objects: SceneObject[] = [];
    public prevObjects: SceneObject[] = [];
    public renderObjects: THREE.Mesh[] = [];

    constructor() {
        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);

        //this.camera = new THREE.OrthographicCamera(window.innerWidth / -200, window.innerWidth / 200, window.innerHeight / 200, window.innerHeight / -200, -500, 1000);
        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setSize(window.innerWidth, window.innerHeight);
        this.renderer.setClearColor( 0xe3e1e1, 0);
        document.body.appendChild(this.renderer.domElement);

        const manager = new THREE.LoadingManager();

        // 2d scene
        // add a plane
        const planeGeometry = new THREE.PlaneGeometry(60, 5);
        const planeMaterial = new THREE.MeshBasicMaterial({ color: 0xd1d1d1, side: THREE.DoubleSide });
        
        const road1 = new THREE.Mesh(planeGeometry, planeMaterial);

        road1.rotation.x = Math.PI / 2;
        road1.position.x = 13;

        this.scene.add(road1);

        const road2 = new THREE.Mesh(planeGeometry, planeMaterial);

        road2.rotation.x = Math.PI / 2;
        road2.position.z = 5;
        road2.position.x = 13;

        this.scene.add(road2);

        const road3 = new THREE.Mesh(planeGeometry, planeMaterial);

        road3.rotation.x = Math.PI / 2;
        road3.position.z = -5;
        road3.position.x = 13;

    
        this.scene.add(road3);

        // add a plane that separates the road
        const planeGeometry2 = new THREE.PlaneGeometry(60, 0.1);
        const planeMaterial2 = new THREE.MeshBasicMaterial({ color: 0x828181, side: THREE.DoubleSide });

        const roadSeparator1 = new THREE.Mesh(planeGeometry2, planeMaterial2);
        
        roadSeparator1.position.y = 0.1;
        roadSeparator1.position.z = 2.5;
        roadSeparator1.rotation.x = Math.PI / 2;
        roadSeparator1.position.x = 13;

        this.scene.add(roadSeparator1);
        
        const roadSeparator2 = new THREE.Mesh(planeGeometry2, planeMaterial2);

        roadSeparator2.position.y = 0.1;
        roadSeparator2.position.z = -2.5;
        roadSeparator2.rotation.x = Math.PI / 2;
        roadSeparator2.position.x = 13;

        this.scene.add(roadSeparator2);

        // create a grid helper
        const gridHelper = new THREE.GridHelper(5, 10);

        //this.scene.add(gridHelper);

        // create a grid

        // move the camera to a top view
        //this.camera.position.z = 5;
        this.camera.position.y = 3;

        this.camera.position.x = -7;
        this.camera.lookAt(0, 0, 0);

        // rotate the camera 90 degrees
        this.camera.rotation.y = Math.PI / 2;

        const controls = new OrbitControls(this.camera, this.renderer.domElement);

        controls.update();

        // light up the scene
        const light = new THREE.AmbientLight(0x404040, 30); // soft white light
        this.scene.add(light);

        const carLoaderObj = new OBJLoader(manager);
        const carLoaderMtl = new MTLLoader(manager);

        // load the mtl file
        carLoaderMtl.load("./src/models/kocsi_final.mtl", (materials) => {
            materials.preload();

            // load the obj file
            carLoaderObj.setMaterials(materials);
            carLoaderObj.load("./src/models/kocsi_final.obj", (object) => {
                object.position.y = .45;
                // add the object to the scene
                this.scene.add(object);
            });
        });

        manager.onLoad = () => {
            console.log("loaded");
            document.getElementById("loadingScreen").style.display = "none";
        };

        const truckLoaderObj = new OBJLoader(manager);
        const truckLoaderMtl = new MTLLoader(manager);

        truckLoaderMtl.load("./src/models/truck_fin2.mtl", (materials) => {
            materials.preload();

            // load the obj file
            truckLoaderObj.setMaterials(materials);
            truckLoaderObj.load("./src/models/truck_fin2.obj", (object) => {
                object.position.y = .28;
                object.position.x = 5;
                object.position.z = 5;

                object.scale.set(1.5, 1.5, 1.5);

                object.rotation.y = Math.PI / 2;

                //object.position.x = 5;
                // add the object to the scene
                this.scene.add(object);
            });
        });

        const fog = new THREE.Fog(0xffffff, 0, 55);
        this.scene.fog = fog;



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

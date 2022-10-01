// import three
import * as THREE from "three";


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

    getCarObject() {
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
}

export { SceneObject };

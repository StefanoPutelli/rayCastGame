const M_PI = 3.14159265358979323846;
const INT_MAX = 2147483647;

export class Map2D {
    static map2D = [];
    height = 0;
    width = 0;
    static map_copy = [];
    constructor(map_text) {
        let buf = map_text.split("\n");
        this.map2D = buf.map((row) => row.split(''));
        let dim = this.setDimensions();
        this.height = dim.maxColLenght;
        this.width = dim.maxRowLenght;
        this.resetMapCopy();
    }
    setDimensions() {
        let maxRowLenght = 0;
        let maxColLenght = 0;
        this.map2D.forEach((row, i) => {
            if (row.length > maxRowLenght) {
                maxRowLenght = row.length;
            }
            if (i > maxColLenght) {
                maxColLenght = i;
            }
        })
        return { maxRowLenght, maxColLenght };
    }
    printMap() {
        this.map2D.forEach((row) => {
            console.log(row.join(''));
        })
    }
    resetMapCopy() {
        this.map_copy = this.map2D.map((row) => row.slice());
    }
    getMap() {
        return this.map2D;
    }
    getDimensions() {
        return { height: this.height, width: this.width };
    }

}

export class RayCaster {
    direction = 0;
    position = { x: 0, y: 0 };
    FOV = 0;
    fov_array = [];
    RESOLUTION = 0;
    dimension = { width: 0, height: 0 };
    map2D = null;
    tile_size = 0;
    map_height = 0;
    map_width = 0;
    constructor(conf, map2D) {
        this.FOV = conf.FOV;
        this.direction = conf.initial_direction;
        this.dimension.height = conf.dimensions.height;
        this.dimension.width = parseInt(conf.dimensions.width/conf.FOV)*conf.FOV;
        this.RESOLUTION = parseInt(this.dimension.width / this.FOV);
        this.map2D = map2D;
        this.tile_size = conf.tile_size;
        let map_dim = this.map2D.getDimensions();
        this.map_height = map_dim.height;
        this.map_width = map_dim.width;
        let initialPosition = this.findPlayer();
        this.position.x = initialPosition.x;
        this.position.y = initialPosition.y;
        console.log("RayCaster constructor");
        console.log("direction " + this.direction)
        console.log("position x " + this.position.x + " y " + this.position.y)
        console.log("FOV " + this.FOV)
        console.log("dimension height " + this.dimension.height + " width " + this.dimension.width)
        console.log("RESOLUTION " + this.RESOLUTION)
        console.log("tile_size " + this.tile_size)
        console.log("----------------------")
    }

    getDecimals(num) {
        return num % 1;
    }

    setDimensions(width, height, fov) {
        this.dimension.width = parseInt(width/fov)*fov;
        this.dimension.height = height;
    }


    findPlayer() {
        let player = { x: 0, y: 0 };
        this.map2D.map2D.forEach((row, i) => {
            row.forEach((col, j) => {
                if (col === 'P') {
                    player.x = j;
                    player.y = i;
                }
            })
        })
        return player;
    }

    getDirVars(angle, playerX, playerY) {
        if (angle < 90) {
            return {
                "x": this.tile_size - this.getDecimals(playerX),
                "y": this.getDecimals(playerY)
            };
        } else if (angle < 180) {
            return {
                "x": this.getDecimals(playerX),
                "y": this.getDecimals(playerY)
            };
        } else if (angle < 270) {
            return {
                "x": this.getDecimals(playerX),
                "y": this.tile_size - this.getDecimals(playerY)
            };
        } else {
            return {
                "x": this.tile_size - this.getDecimals(playerX),
                "y": this.tile_size - this.getDecimals(playerY)
            };
        }
    }

    saveInTheFov(index, dist) {
        this.fov_array[this.dimension.width - index] = Math.abs(dist);
    }

    markBlock(map2D, x, y, marker) {
        map2D.map_copy[y][x] = marker;
    }

    getFovArray() {
        return this.fov_array;
    }

    turn(value) {
        if (value > 0) {
            this.direction = this.direction + value >= 360 ? this.direction = 0 + this.direction + value - 360 : this.direction + value;
        }else if (value < 0) {
            this.direction = this.direction + value < 0 ? this.direction = 360 + this.direction + value : this.direction + value;
        }else{
            return;
        }
    }

    rayCastInTheFov() {
        this.map2D.resetMapCopy();
        let start = parseInt(this.direction - (this.FOV / 2));
        let end = parseInt(this.direction + (this.FOV / 2));
        for (let i = start; i < end; i++) {
            for (let r = 0; r < this.RESOLUTION; r++) {
                let pre_angle = i % 360 + r / this.RESOLUTION;
                let angle = pre_angle < 0 ? pre_angle + 360 : pre_angle;
                let dirVal = this.getDirVars(angle, this.position.x, this.position.y);
                let angle_rad = (angle * M_PI / 180);
                let sinAngle = Math.sin(angle_rad);
                let cosAngle = Math.cos(angle_rad);
                let sinSign = sinAngle > 0 ? 1 : -1;
                let cosSign = cosAngle > 0 ? 1 : -1;
                let dptX = 0;
                let dptY = 0;
                let x_distance = 0;
                let y_distance = 0;
                while (true) {
                    if (angle === 0 || angle === 180) {
                        y_distance = INT_MAX;
                    } else {
                        y_distance = ((dirVal.y + (dptY * this.tile_size)) / Math.abs(sinAngle));
                    }
                    if (angle === 90 || angle === 270) {
                        x_distance = INT_MAX;
                    } else {
                        x_distance = ((dirVal.x + (dptX * this.tile_size)) / Math.abs(cosAngle));
                    }
                    let fov_index = (i - start) * this.RESOLUTION + r;
                    let fish_eye_correction = Math.cos((fov_index / this.RESOLUTION - this.FOV / 2) * M_PI / 180);
                    if (x_distance < y_distance) {
                        let dX = parseInt(this.position.x + (x_distance * cosAngle + this.tile_size / 2 * cosSign));
                        let dY = parseInt(this.position.y - (x_distance * sinAngle));
                        dptX += 1;
                        if (dY < 0 || dY >= this.map_height || dX < 0 || dX >= this.map_width) {
                            this.saveInTheFov(fov_index, -1);
                            break;
                        }
                        if (this.map2D.map2D[dY][dX] === '#') {
                            this.markBlock(this.map2D, dX, dY, 'X');
                            this.saveInTheFov(fov_index, Math.abs(x_distance * fish_eye_correction));
                            break;
                        }

                    } else {
                        let dX = parseInt(this.position.x + (y_distance * cosAngle));
                        let dY = parseInt(this.position.y - (y_distance * sinAngle + this.tile_size / 2 * sinSign));
                        dptY += 1;
                        if (dY < 0 || dY >= this.map_height || dX < 0 || dX >= this.map_width) {
                            this.saveInTheFov(fov_index, -1);
                            break;
                        }
                        if (this.map2D.map2D[dY][dX] === '#') {
                            this.markBlock(this.map2D, dX, dY, 'Y');
                            this.saveInTheFov(fov_index, Math.abs(y_distance * fish_eye_correction));
                            break;
                        }
                    }
                }
            }
        }
    }
}

export class Screen {
    dimension = {
        height: 0,
        width: 0,
        margin: 0
    };
    margin = 0;
    constructor(conf) {
        this.dimension.height = conf.dimensions.height;
        this.dimension.width = parseInt(conf.dimensions.width/conf.FOV)*conf.FOV;
        this.margin = parseInt((conf.dimensions.width - this.dimension.width)/2);
    }

    setDimensions(width, height) {
        this.dimension.height = height;
        this.dimension.width = width;
    }

    drawScreen(canvas, fov_array, map2D) {
        let ctx = canvas.getContext("2d");
        let screenCenter = this.dimension.height / 2;
        ctx.clearRect(0, 0, this.dimension.width, this.dimension.height);
        for(let i = 0; i < fov_array.length; i++) {
            let halfHeight = screenCenter / fov_array[i];
            let start = screenCenter - halfHeight;
            let end = screenCenter + halfHeight;
            ctx.strokeStyle = "rgba(0, 0, 255, " + 1/fov_array[i] + ")";
            ctx.beginPath();
            ctx.moveTo(i + this.margin, start);
            ctx.lineTo(i + this.margin, end);
            ctx.stroke();
        }

        //TODO: draw map
    }

}
import {output,input} from "./index.js";

const { d13, a27 } = output('192.168.1.138', 'd13', 'a27');

const {d3,a34} = input('192.168.1.138', 'd3','a34');

d13.set('high');
a27.set(2);

console.log(await d3.get());
console.log(await a34.get());
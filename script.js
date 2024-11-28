
let query = `https://overpass-api.de/api/interpreter?data=` +`[out:json][timeout:25];
node(around:10,40.742218,-73.989127);
//node(1538138815);
  way(bn)["highway"~"^(motorway|trunk|primary|secondary|tertiary|unclassified|residential|service|living_street)$"];
  out geom`;

fetch(query).then((response ) => response.json())
.then((data)=>{
    console.log(data);
    
}).catch((error)=>{
    console.log(error);
    
})

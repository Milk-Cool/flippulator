const fs = require("fs");

const file = process.argv[process.argv.length - 1];
const obj = file.replaceAll(".c", ".o");

const timestamps = require("./timestamps.json");
const time = timestamps[file];

const et = Math.floor(fs.statSync(file).ctime / 1000);

if(!fs.existsSync(obj) || time != et)
    console.log(JSON.stringify({
        "obj": obj,
        "time": et
    }));
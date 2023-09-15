#!/usr/bin/env node

const util = require("node:util");
const exec = util.promisify(require("node:child_process").exec);
const { spawn } = require("node:child_process");
const readline = require("readline");
const fs = require("fs-extra");
const { join } = require("path");

const rl = readline.createInterface({
    "input": process.stdin,
    "output": process.stdout
});

const question = text => new Promise(resolve => rl.question(text, resolve));

const APP_COPY = "flippulator_app_copy";

(async () => {
    const folder = await question("Enter the application folder name: ");
    if(!fs.existsSync(folder)) {
        console.log("Folder does not exist!");
        process.exit(1);
    }
    if(!fs.existsSync(join(folder, "application.fam"))) {
        console.log("Manifest does not exist!");
        process.exit(2);
    }
    fs.copySync(folder, APP_COPY, { "overwrite": true });
    const { stdout } = await exec("python3 manifest.py " + join(APP_COPY, "application.fam"));
    const manifest = JSON.parse(stdout.trim());
    fs.appendFileSync(join(APP_COPY, manifest.appid + ".c"), `
int main() {
    furi_init();
    ${manifest.entry_point}(NULL);
}`);
    await exec("make clean");
    spawn("make", [], {
        "cwd": process.cwd(),
        "detached": false,
        "stdio": "inherit"
    }).on("close", () => {
        console.log("Done!");
        process.exit(0);
    });
})();
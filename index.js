#!/usr/bin/env node

const util = require("node:util");
const exec = util.promisify(require("node:child_process").exec);
const { spawn } = require("node:child_process");
const readline = require("readline");
const fs = require("fs");
const { join } = require("path");
const { tmpdir } = require("os");

const file = process.argv[process.argv.length - 1];

const rl = readline.createInterface({
    "input": process.stdin,
    "output": process.stdout
});

const question = text => new Promise(resolve => rl.question(text, resolve));

const APP_COPY = "flippulator_app_copy";

(async () => {
    let folder;
    if(file != __filename && fs.existsSync(file))
        folder = file;
    else {
        folder = await question("Enter the application folder name: ");
        if(!fs.existsSync(folder)) {
            console.log("Folder does not exist!");
            process.exit(1);
        }
    }
    if(!fs.existsSync(join(folder, "application.fam"))) {
        console.log("Manifest does not exist!");
        process.exit(2);
    }
    await exec("make clean");
    // fs.copySync(folder, APP_COPY, { "overwrite": true });
    fs.mkdirSync(APP_COPY);
    await exec(`cp -r "${folder}"/* "${APP_COPY}"`);
    const { stdout } = await exec("python3 manifest.py " + join(APP_COPY, "application.fam"));
    const manifest = JSON.parse(stdout.trim());
    fs.writeFileSync(join(APP_COPY, manifest.appid + ".c"), `
#include <termios.h>
#include <signal.h>
extern struct termios global_old_tio, global_new_tio;

` + fs.readFileSync(join(APP_COPY, manifest.appid + ".c"), "utf-8") + `
void sigint_handler(int p) {
    UNUSED(p);
    // do nothing
    // exit_sdl(0);
}
int main() {
    signal(SIGINT, sigint_handler);
    tcgetattr(STDIN_FILENO, &global_old_tio);
    global_new_tio = global_old_tio;
    global_new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &global_new_tio);
    furi_init();
    
    FuriThread* thr = furi_thread_alloc();
    furi_thread_set_callback(thr, ${manifest.entry_point});
    furi_thread_set_stack_size(thr, ${manifest.stack_size});
    furi_thread_start(thr);
    furi_thread_join(thr);
    
    exit_sdl(0);
}`);
    fs.writeFileSync(join(tmpdir(), "flippulator_temp_out_app_dir"), "out_" + manifest.appid + "/");
    fs.writeFileSync(join(tmpdir(), "flippulator_temp_out_app_name"), manifest.appid);
    fs.writeFileSync(join(tmpdir(), "flippulator_temp_cc_extra"), "\"-DFLIPPULATOR_APP_NAME=\\\"" + (manifest.name || "Flippulator") + "\\\"\"");
    
    spawn("make", [], {
        "cwd": process.cwd(),
        "detached": false,
        "stdio": "inherit"
    }).on("close", () => {
        console.log("Done!");
        process.exit(0);
    });
})();
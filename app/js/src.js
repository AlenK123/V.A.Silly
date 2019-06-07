const path = require('path');
var exec = require('child_process').exec;

function run() {
    var title = document.getElementById("run-button");

    var threads = document.getElementById("threads");

    var input = "0";

    var n_threads = Number(threads.value);

    if (isNaN(n_threads)) {
        document.getElementById("thread-err").innerHTML = "Invalid input";
        return;
    }

    if (n_threads < 0) {
        document.getElementById("thread-err").innerHTML = "Threads number can\'t be negetive";
        return;
    }

    var run_str = "./build" + " " + input + " " + n_threads;

    process.chdir("../image-processing/src");
    var id = exec(run_str, function(err, stdout, stderr) {
        console.log(err);
    });

    process.chdir(__dirname);
}
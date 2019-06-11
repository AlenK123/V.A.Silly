
const exec = require('child_process').exec;
const exists = require('fs').existsSync;

var media = "0";

function switch_class() {
    var elem = document.getElementById("nthr");
    elem.classList.add("active");
}

function disable_file_button() {
    document.getElementById("file-btn").classList.add("disabled");
    media = "0";
}

function disable_camera_button() {
    document.getElementById("cam-btn").classList.add("disabled");
    media = NaN;
}

function run() {
    var threads = document.getElementById("n-threads");
    var run_btn = document.getElementById("run-btn");

    var n_threads = Number(threads.value);

    if (isNaN(n_threads)) {
        document.getElementById("doc").innerHTML = "Invalid input";
        return;
    }

    if (n_threads < 0) {
        document.getElementById("doc").innerHTML = "Threads number can\'t be negetive";
        return;
    }

    if (n_threads == 0) {
        document.getElementById("doc").innerHTML = "Threads number can\'t be 0";
        return;
    }


    if (isNaN(media)) {
        var path = document.getElementById("file-path");
        
        if (!exists(path.value)) {
            document.getElementById("file-des").innerHTML = "The File Doesn\'t exist!";
            return;
        }

        media = path.value;
    }

    var run_str = "./build" + " " + media + " " + n_threads;

    run_btn.innerHTML = "Stop";

    run_btn.onclick = (function() {
        return function() {
            exec("pkill build", function(err, stdout, stderr) {
                console.log(err);
            });
            
            var run_btn = document.getElementById("run-btn");
            run_btn.innerHTML = "Run";
            run_btn.onclick = run;
        }
    })();

    process.chdir("../image-processing/src");
    exec(run_str, function(err, stdout, stderr) {
        console.log(err);
    });

    process.chdir(__dirname);
}
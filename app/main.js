const electron = require('electron');
const url = require('url');
const path = require('path');
var exec = require('child_process').exec;

const { app, BrowserWindow, Menu, ipcMain, systemPreferences } = electron;

let mainWindow;
let calWindow;
let status = false;

/* Closes the current square size window as well as the program process */
function closeWin(s) {
    dir = exec("ps -ef | grep ./gl | grep -v grep | awk '{print $2}'  | xargs kill ", function(err, stdout, stderr) { console.log(stderr); });
    if (calWindow)
        calWindow.close();
    status = s;
    size();
}

app.on('ready', function() {
    mainWindow = new BrowserWindow({
        webPreferences: { nodeIntegration: true },
        titleBarStyle: 'hiddenInset',
        resizable: false,
        maximizable: false
    });
    /* Compiling the program */
    dir = exec("cd ../src/ && make", function(err, stdout, stderr) {
        console.log(stdout);
    });
    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, "index.html"),
        protocol: 'file:',
        slashes: true
    }));
    mainWindow.on('closed', function() {
        dir = exec("ps -ef | grep ./gl | grep -v grep | awk '{print $2}'  | xargs kill ", function(err, stdout, stderr) { console.log(stderr); });
        app.quit();
    });

    const mainMenu = Menu.buildFromTemplate(mainMenuTem);
    Menu.setApplicationMenu(mainMenu);
});

/* Open square size input window */
function size() {
    calWindow = new BrowserWindow({
        webPreferences: { nodeIntegration: true },
        width: 700,
        resizable: false,
        maximizable: false,
        height: 250,
        titleBarStyle: 'hiddenInset'
    });
    calWindow.loadURL(url.format({
        pathname: path.join(__dirname, "size.html"),
        protocol: 'file:',
        slashes: true
    }));
    calWindow.on('closed', function() {
        calWindow = null;

    })
}

/* Function called after input of square size */
ipcMain.on('close', function(e, item) {
    if (!status)
        dir = exec("../src/gl c " + item, function(err, stdout, stderr) {
            console.log(stdout);
        });
    else
        dir = exec("../src/gl r " + item, function(err, stdout, stderr) { console.log(stdout); });
    console.log(item);
    calWindow.close();
});

/* Marker Detection */
ipcMain.on('marker', function(e, item) {
    closeWin(true);
});

/* Calibration */
ipcMain.on('calibration', function(e, item) {
    closeWin(false);
});

/* Menu Items */
const mainMenuTem = [{
        label: 'Program',
        submenu: [{
                label: 'Settings',
                accelerator: process.platform == 'darwin' ? 'Cmd+,' : 'Ctrl+,',
            },
            {
                label: 'Developer',
                accelerator: process.platform == 'darwin' ? 'Cmd+I' : 'Ctrl+I',
                click(item, focusedWindow) {
                    focusedWindow.toggleDevTools();
                }

            }

        ]
    },
    {
        label: 'Options',
        submenu: [{
                label: 'Calibration',
                accelerator: process.platform == 'darwin' ? 'Option+C' : 'Alt+C',
                click() {
                    closeWin(false);
                }
            },
            {
                label: 'Run',
                accelerator: process.platform == 'darwin' ? 'Option+R' : 'Alt+R',
                click() {
                    closeWin(true);
                }
            },
            {
                label: 'Quit',
                accelerator: process.platform == 'darwin' ? 'Cmd+Q' : 'Ctrl+Q',
                click() {
                    dir = exec("ps -ef | grep ./gl | grep -v grep | awk '{print $2}'  | xargs kill ", function(err, stdout, stderr) { console.log(stderr); });
                    app.quit();
                }
            }
        ]
    }
];
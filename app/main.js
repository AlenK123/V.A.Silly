const electron = require('electron');
const url = require('url');
const path = require('path');
var exec = require('child_process').exec;
const dialog = require('electron').dialog;

const { app, BrowserWindow, Menu, ipcMain, systemPreferences } = electron;

let mainWindow;

ipcMain.on('open-file-dialog', function (event) {
    dialog.showOpenDialog({
        properties: ['openFile']
    }, function (files) {
        if (files) event.sender.send("file-path", files);
    });
});

app.on('ready', function() {
    mainWindow = new BrowserWindow({
        webPreferences: { nodeIntegration: true },
        titleBarStyle: 'hiddenInset',
        resizable: false,
        maximizable: false
    });
    /* Compiling the program */
    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, "index.html"),
        protocol: 'file:',
        slashes: true
    }));
    mainWindow.on('closed', function() {
        dir = exec("ps -ef | grep VAS_exec | grep -v grep | awk '{print $2}'  | xargs kill ", function(err, stdout, stderr) { console.log(stderr); });
        app.quit();
    });

    const mainMenu = Menu.buildFromTemplate(mainMenuTem);
    Menu.setApplicationMenu(mainMenu);
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
                label: 'Kalil',
                accelerator: process.platform == 'darwin' ? 'Option+C' : 'Alt+C',
                click() {
                    closeWin(false);
                }
            },
            {
                label: 'Kal',
                accelerator: process.platform == 'darwin' ? 'Option+R' : 'Alt+R',
                click() {
                    closeWin(true);
                }
            },
            {
                label: 'Quit',
                accelerator: process.platform == 'darwin' ? 'Cmd+Q' : 'Ctrl+Q',
                click() {
                    dir = exec("ps -ef | grep VAS_exec | grep -v grep | awk '{print $2}'  | xargs kill ", function(err, stdout, stderr) { console.log(stderr); });
                    app.quit();
                }
            }
        ]
    }
];
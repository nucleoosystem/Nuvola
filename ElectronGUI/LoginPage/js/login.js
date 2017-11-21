const electron = require('electron');
const app = require('electron')
const BrowserWindow = electron.remote.BrowserWindow;
const remote = require('electron').remote;


// TODO: add signup function
function handleFormData() {
    
    var username = document.getElementById("username").value; // Get the values of the form
    var password = document.getElementById("password").value;
    
    //var connection = new WebSocket('ws://127.0.0.1:22223');
    /*connection.onopen() = function {
        connection.send("ping");
    }*/
    
    openMainPage();
}

$(document).ready(function() {
    $("#closeImage").click(function(){
        var window = remote.getCurrentWindow();
        window.close();
    }); 
});


function openMainPage() {
    var fileName = __dirname + '/../MainPage/main.html';
    var window = remote.getCurrentWindow();
    
    let new_win
    
    new_win = new BrowserWindow({width: 1920, height: 1080, frame: true, show: false});
    new_win.maximize();
    new_win.setMenu(null);
    new_win.loadURL(fileName);
    //new_win.webContents.openDevTools()
    
    new_win.webContents.on('did-finish-load', function() {
        window.close();
        new_win.show();
    });    
}
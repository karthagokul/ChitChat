var output;

var websocket;

function WebSocketSupport() {
    if (browserSupportsWebSockets() === false) {
        document.getElementById("ws_support").innerHTML = "<h2>Sorry! Your web browser does not supports web sockets</h2>";

        var element = document.getElementById("wrapper");
        element.parentNode.removeChild(element);

        return;
    }

    output = document.getElementById("chatbox");
}

function onMessage(e) {
    var obj = JSON && JSON.parse(e.data) || $.parseJSON(jse.dataon);
    if (obj.command == "chat") {
        writeToScreen('<span style="color: blue;"> ' + '<b></span><span style="color: black;">' + obj.sender + '</b>:' + obj.message + '</span>');
    }else if (obj.command == "mention") {
        writeToScreen('<span style="color: blue;"> ' + '<b></span><span style="color: black;">' + obj.sender + '</b>:' + obj.message + '</span>');
    } else if (obj.command == "online") {
        //writeToScreen('<span style="color: blue;">online status changed</span>');
        updateBuddies(obj);

    } else if (obj.command == "logoff") {
        writeToScreen('<span style="color: blue;">' + obj.message + 'Logged Off! </span>');
        updateBuddies(obj);
    } else if (obj.command == "logon") {
        //writeToScreen('<span style="color: blue;">' + obj.sender + 'Logged In! </span>');
    } else {
        //writeToScreen('<span style="color: blue;"> NOT Implemented </span>');
        // ToDo : Fix me
        updateBuddies(obj);
    }
}

function updateBuddies(obj) {
    var listDiv = document.getElementById('list-chat');

    //Deleting all child nodes
    while (listDiv.firstChild) {
        listDiv.removeChild(listDiv.firstChild);
    }

    //Now Lets update the buddies
    var ul = document.createElement('ul');
    listDiv.appendChild(ul);
    var lh = document.createElement('lh');
    var header = document.createTextNode("Online Buddies");
    lh.appendChild(header);
    ul.appendChild(lh);
    for (var i in obj.buddies) {
        var li = document.createElement('li');
        var id = obj.buddies[i];
        var textnode = document.createTextNode(id);
        li.appendChild(textnode);
        ul.appendChild(li);
    }
}

function doLogin(server, username) {
    websocket = new WebSocket('ws:' + server + ':9090');
    websocket.onopen = function(e) {
        writeToScreen("You have have successfully connected to the server");
        var chatname = document.getElementById('username').value;
        //var msg = '@<b>' + chatname + '</b>: ' + message;
        var obj = new Object();
        obj.command = "logon";
        obj.sender = chatname;
        var jsonString = JSON.stringify(obj);
        websocket.send(jsonString);
    };
    websocket.onmessage = function(e) {
        onMessage(e)
    };
    websocket.onerror = function(e) {
        onError(e)
    };
}

function doSend(message) {
    var chatname = document.getElementById('username').value;
    document.getElementById('msg').value = "";
    document.getElementById('msg').focus();
    var obj = new Object();
    obj.command = "chat";
    obj.sender = chatname;
    obj.message = message;
    var jsonString = JSON.stringify(obj);
    websocket.send(jsonString);
}

function writeToScreen(message) {
    var pre = document.createElement("p");
    pre.style.wordWrap = "break-word";
    pre.innerHTML = message;
    output.appendChild(pre);
}

function onError(e) {
    writeToScreen('<span style="color: red;">ERROR:</span> ' + e.data);
}

function browserSupportsWebSockets() {
    if ("WebSocket" in window) {
        return true;
    } else {
        return false;
    }
}

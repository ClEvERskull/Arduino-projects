let stompClient = null;
//const serverWebSocketUrl = 'http://localhost:8080/iot';
const applianceSerialNumber = '';
const serverWebSocketUrl = 'https://bp-smart-env-api.herokuapp.com/iot';
const serverHostUrl = 'https://bp-smart-env-api.herokuapp.com';


function connect() {
    let socket = new SockJS(serverWebSocketUrl);
    stompClient = Stomp.over(socket);
    stompClient.connect({ 'applianceSerialNumber': applianceSerialNumber }, function (frame) {
        //setConnected(true);
        console.log('Connected: ' + frame);


        stompClient.subscribe(`/topic/appliance/Termostat01`, function (messageOutput) {
            //showMessageOutput(JSON.parse(messageOutput.body));
            console.log(JSON.parse(messageOutput.body));

            data = JSON.parse(messageOutput.body);
            //showMessages(messageOutput);
            showMessageOutput(JSON.parse(messageOutput.body));
            /*
            if (data["applianceSerialNumber"] == "Temp") {
                GetTempFromDevice(data);
            }
            */

            switch(data["elementId"]) {
                case "0":
                    GetDeviceData(data, 'temp_living');
                  break;
                case "1":
                    GetDeviceData(data, 'termostat_mode');
                  break;
                case "2":
                    GetDeviceData(data, 'termostat_temp');
                  break;
                default:
                  console.log('Termostat01 vracia divne id');
              } 
/*
            switch(data["applianceSerialNumber"]) {
                case "Termostat01":
                    switch(data["elementId"]) {
                        case "0":
                            GetDeviceData(data, 'temp_living');
                          break;
                        case "1":
                            GetDeviceData(data, 'termostat_mode');
                          break;
                        case "2":
                            GetDeviceData(data, 'termostat_temp');
                          break;
                        default:
                          console.log('Termostat01 vracia divne id');
                      } 
                  break;
                case "Teplomer_von":
                    switch(data["elementId"]) {
                        case "0":
                            GetDeviceData(data, 'temp_out');
                          break;
                        case "1":
                            GetDeviceData(data, 'humidity_out');
                          break;
                        case "2":
                            GetDeviceData(data, 'air_out');
                          break;
                        case "3":
                            GetDeviceData(data, 'daytime');
                          break;
                        default:
                          console.log('Teplomer_von vracia divne id');
                      } 
                  break;
                case "Teplomer_dnu":
                    switch(data["elementId"]) {
                        case "0":
                            GetDeviceData(data, 'temp_hall');
                          break;
                        case "1":
                            GetDeviceData(data, 'lights_hall');
                          break;
                        case "2":
                            GetDeviceData(data, 'air_hall');
                          break;
                        default:
                          console.log('Teplomer_dnu vracia divne id');
                      } 
                  break;
                default:
                  console.log('nepoznam toto zariadenie');
              } 
              */

        });

        stompClient.subscribe(`/topic/appliance/zamok`, function (messageOutput){
            console.log(JSON.parse(messageOutput.body));

            data = JSON.parse(messageOutput.body);
            //showMessages(messageOutput);
            showMessageOutput(JSON.parse(messageOutput.body));
            GetDeviceData(data, 'lock');
        });

        console.log(`Subscribed to '/topic/appliance/${applianceSerialNumber}' to listen for messages`);
    });
}

function GetDeviceData(data, id) {
    console.log("Zariadenie " + data["applianceSerialNumber"] + " vrátilo hodnotu " + data["value"]);

    document.getElementById(id).innerHTML = "" + data["value"];
}

function disconnect() {
    if (stompClient != null) {
        stompClient.disconnect();
    }
    setConnected(false);
    console.log("Disconnected");
}


function ChangeValue(checkbox,  serial, id) {
    let chval = checkbox.checked == true ? 1 : 0;
    console.log("HODNOTA: " + chval);
    if(chval == 0){
        chval = 'Off';
    }
    else if(chval == 1){
        chval = 'On';
    } 


    stompClient.send(`/topic/appliance/${serial}`, {},
        JSON.stringify({
            "applianceSerialNumber": serial,
            "elementId": id,
            "messageType": "VALUE_CHANGE",
            "value": chval
        }
        ));
}


function ChangeTermostat(selection, serial, id) {
    let chval = selection.value;
    console.log("HODNOTA: " + chval);
    stompClient.send(`/topic/appliance/${serial}`, {},
        JSON.stringify({
            "applianceSerialNumber": serial,
            "elementId": id,
            "messageType": "VALUE_CHANGE",
            "value": chval
        }
        ));
}


function ChangeChannel(selection) {
    let chval = selection.selectedIndex + 1;
    console.log("HODNOTA: " + chval);
    stompClient.send(`/topic/appliance/${applianceSerialNumber}`, {},
        JSON.stringify({
            "applianceSerialNumber": "Termostat01",
            "elementId": "2",
            "messageType": "VALUE_CHANGE",
            "value": chval
        }
        ));
}



function ChangeChannelInput(selection) {
    if (window.event.key === 'Enter') {
        let chval = selection.value;
        console.log("HODNOTA: " + chval);
        stompClient.send(`/topic/appliance/${applianceSerialNumber}`, {},
            JSON.stringify({
                "applianceSerialNumber": "Termostat01",
                "elementId": "2",
                "messageType": "VALUE_CHANGE",
                "value": chval
            }
            ));
    }
}
/*
function ChangeTemp(selection) {
    let chval = selection.value;
    console.log("HODNOTA: " + chval);
    stompClient.send(`/topic/appliance/${applianceSerialNumber}`, {},
        JSON.stringify({
            "applianceSerialNumber": applianceSerialNumber,
            "elementId": "2",
            "messageType": "VALUE_CHANGE",
            "value": chval
        }
        ));
}*/

function RegisterDevice(selection) {

    if (window.event.key === 'Enter') {
        let chval = selection.value;
        console.log("HODNOTA: " + chval);
        stompClient.send("/app/iot/register-appliance", { channel: chval },
            JSON.stringify({ 'applianceSerialNumber': chval }));
            stompClient.send(`/topic/appliance/${chval}`, {},
            JSON.stringify({
                "applianceSerialNumber": chval,
                "elementId": 0,
                "messageType": "Info",
                "value": "OK"
            }
            ));
        
    }
}


function CreateHome(desc, name) {
    var xhr = new XMLHttpRequest();
    var url = serverHostUrl + "/home";
    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.onreadystatechange = function () {
        if (xhr.readyState === 4 && xhr.status === 200) {
            var json = JSON.parse(xhr.responseText);
            console.log(json);
        }
    };
    var data = JSON.stringify({  "description": desc, "name": name});
    xhr.send(data);
}



function CreateUser(firstname, lastname, password, email, username) {
    var xhr = new XMLHttpRequest();
    var url = serverHostUrl + "/auth​/login​/create-user";
    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "application/json");
    xhr.onreadystatechange = function () {
        if (xhr.readyState === 4 && xhr.status === 200) {
            var json = JSON.parse(xhr.responseText);
            console.log(json);
        }
    };
    var data = JSON.stringify({  "firstname": firstname,
    "lastname": lastname,
    "mail": email,
    "password": password,
    "username": username});

    xhr.send(data);
}

function showMessageOutput(messageOutput) {

    /*
    const dateTimeFormat = new Intl.DateTimeFormat('UK', { year: 'numeric', month: '2-digit', day: '2-digit',
        hour: '2-digit',minute: '2-digit', second: '2-digit' });

    let timestamp = document.createElement('p');
    timestamp.appendChild(
        document.createTextNode(`timestamp: ${dateTimeFormat.format(new Date())}`),
    );
    */
    let applianceSerialNumber = document.createElement('p');
    applianceSerialNumber.appendChild(
        document.createTextNode(`applianceSerialNumber: ${messageOutput.applianceSerialNumber}`),
    );
    let messageType = document.createElement('p');
    messageType.appendChild(
        document.createTextNode(`messageType: ${messageOutput.messageType}`),
    );
    let elementId = document.createElement('p');
    elementId.appendChild(
        document.createTextNode(`elementId: ${messageOutput.elementId}`),
    );

    let value = document.createElement('p');
    value.appendChild(
        document.createTextNode(`value: ${messageOutput.value}`),
    );

    let div = document.createElement('div');
    div.style.wordWrap = 'break-word';
    div.append(applianceSerialNumber, messageType, elementId, value, document.createElement('hr'));

    let response = document.getElementById('response');
    response.appendChild(div);
};
/*
function showMessages(data){
    document.getElementById("spravy").innerHTML = JSON.stringify(data.body) + "\n";
}
*/

connect();

//CreateUser("Jan", "Hruska", "123456789", "test@test.sk", "jhruska123");

//CreateHome("Domov1", "Domov1");



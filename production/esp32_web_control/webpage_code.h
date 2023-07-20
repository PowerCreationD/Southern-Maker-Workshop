const char webpageCode[] =
R"=====(
<!DOCTYPE html>
<html>
<head>
    <style>
        .command-area {
            display: flex;
            flex-direction: column;
            margin: 0 auto;
            align-items: center;
        }
        .btn
        {   display: flex;
            width: 80px;
            height: 40px;
            color: aliceblue;
            background: rgb(40, 75, 216);
            font: 20px;
            border-radius: 5px;
            cursor: pointer;
            align-items: center;
            justify-content: center;
            margin-bottom: 10px;
        }
    </style>
</head>
<body>
    
    <div class="command-area">
        <h1>CuGo V3 control</h1>
        <button class="btn" onclick="turnOn()">ON</button>
        <button class="btn" onclick="turnOff()">OFF</button>
        <button class="btn" onclick="goForward()">Forward</button>
        <button class="btn" onclick="goBackward()">Backward</button>
    </div>
    <script>
        const url = window.location.host
        const turnOn = () => {

            return fetch(`http://${url}/on`, {
                method: "GET"
            })
            .then( response => {
                data = response.json()
                console.log(data)
            })
            .catch(error => 
                console.error(error)
            )
        }
        const turnOff = () => {

            return fetch(`http://${url}/off`, {
                method: "GET"
            })
            .then( response => {
                data = response.json()
                console.log(data)
            })
            .catch(error => 
                console.error(error)
            )
        }
        const goForward = () => {

            return fetch(`http://${url}/forward`, {
                method: "GET"
            })
            .then( response => {
                data = response.json()
                console.log(data)
            })
            .catch(error => 
                console.error(error)
            )
        }
        const goBackward = () => {

            return fetch(`http://${url}/backward`, {
                method: "GET"
            })
            .then( response => {
                data = response.json()
                console.log(data)
            })
            .catch(error => 
                console.error(error)
            )
        }
    </script>
</body>
</html>
)=====";
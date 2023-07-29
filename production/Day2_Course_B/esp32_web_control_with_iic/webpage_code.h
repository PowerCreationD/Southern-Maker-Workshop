const char webpageCode[] =
R"=====(
<!DOCTYPE html>
<html>
<!DOCTYPE html>
<html>
<head>
    <style>
        .command-area {
            display: flex;
            width: 100%;
            flex-direction: column;
            margin: 0 auto;
            align-items: center;
        }
        .btn
        {   display: flex;
            width: 20%;
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
    </div>
    <script>
        const url = window.location.host
        function turnOn() {

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
        function turnOff() {

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
    </script>
</body>
</html>
)=====";
void handle_main_page(){
  String html;
  html = R"rawliteral(
    <!DOCTYPE html>
    <html>
      <head>
        <input type="button" onclick="window.location.href='/setup'" value="change network credentials">
        <h2>Email Sender</h2>
      </head>
      <body>
        <label for="email_receiver">Email Recipient:</label>
        <input type="email" id="email_receiver" required>
        <br>
        <input type="button" onclick="sendemail()" value="Send">
      </body>
      <script>
      function sendemail(){
        alert("Email sent");
        var recipient = document.getElementById("email_receiver").value;
        fetch('/email?emailaddress=' + recipient)
          .then(response => response.text())
          .then(data => {
            
            //functions here
          })
          .catch(error => console.error('Error sending email: ', error));
        }
      </script>
    </html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handle_setup_page(){
  String html;
  html = R"rawliteral(
  <!DOCTYPE html>
  <html>
    <body>
      <h1>WiFi credentials</h1>
      <form id="savewifi">
        <label for="ssid">SSID</label>
        <input type="text" id="ssid" maxlength="32" required>
        <br>
        <label for="password">Password</label>
        <input type="password" id="password" maxlength="32">
        <br>
        <input type="submit" value="save">
      </form>
    </body>

    <script>
      document.getElementById("savewifi").addEventListener("submit", function(e){
        e.preventDefault();
        saveinput();
      });

      function saveinput(){
        var ssid = document.getElementById("ssid").value;
        var pass = document.getElementById("password").value;

        fetch('/save_creds?ssid=' + ssid + '&pass=' + pass)
          .then(response => response.text())
          .then(data => {
            alert("Saved the WiFi");
            //put functions here
          })
          .catch(error => console.error('Error fetching save ssid',error));
      }
    </script>
  </html>
  )rawliteral";
  server.send(200, "text/html", html);
}
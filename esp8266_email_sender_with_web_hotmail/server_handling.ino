void setupserver()
{
  if(credentials_exist)
  {
    server.on("/", handle_main_page);
    server.on("/setup",handle_setup_page);
  }
  else
  {
    server.on("/", handle_setup_page);
  }

  server.on("/save_creds", save_creds);
  server.on("/email", handle_email);
}

void save_creds(){
  if(server.hasArg("ssid"))
  {
    String newssid = server.arg("ssid");
    String newpass = "";

    if(server.hasArg("pass"))
      newpass = server.arg("pass");
    
    save_credentials_EEPROM(newssid, newpass);

    Serial.println("SSID: " + newssid + "\n" + "Password: " + newpass);
    wifimode();
    server.send(200, "text/plain", "Connected");
  }
  else
    server.send(400,"text/plain", "Not Connected");
}

void handle_email() {
  if (server.hasArg("emailaddress")) {
    receivedEmail = server.arg("emailaddress");
    Serial.println("Received Email: " + receivedEmail);

    sendEmail();  // Send email to user

    server.send(200, "text/plain", "Email sent to: " + receivedEmail);
  } else {
    server.send(400, "text/plain", "Email not received.");
  }
}

void sendEmail() {
  smtp.debug(1);

  smtp.callback([](SMTP_Status status) {
    Serial.println(status.info());
  });
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
  SMTP_Message message;
  message.sender.name = "ESP8266 Mailer";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP8266 Email Test";
  message.addRecipient("User", receivedEmail.c_str());
  message.text.content = "Hello! This is a test email sent from ESP8266.  ";
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session))
    return;

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Email send failed" + smtp.errorReason());
  } else {
    Serial.println("Email sent successfully!");
  }
}
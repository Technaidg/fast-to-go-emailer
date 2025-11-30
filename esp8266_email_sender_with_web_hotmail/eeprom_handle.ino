void save_credentials_EEPROM(const String newssid,const String newpass)
{

  // write a 0 to all 64 bytes of the EEPROM
  for (int i = 0; i < 64; i++) { EEPROM.write(i, 0); }

  int ssid_length = newssid.length();
  int pass_length = newpass.length();
  
  if(ssid_length  > 32|| pass_length > 32)
  {
    Serial.print("pass or ssid is too large");
    return;
  }

  for(int i = 0; i<ssid_length; i++)
  {
    byte current_char = char(newssid.charAt(i));
    EEPROM.write(ssid_index + i, current_char);
  }
  for(int i = ssid_length + ssid_index; i<ssid_index+32; i++)
    EEPROM.write(i, 0);
  
  for(int i = 0; i<pass_length; i++)
  {
    byte current_char = char(newpass.charAt(i));
    EEPROM.write(pass_index + i, current_char);
  }
  for (int i = pass_length + pass_index; i<pass_index+32; i++)
    EEPROM.write(i, 0);

  EEPROM.commit();

  ssid = newssid;
  password = newpass;
}

String read_EEPROM(int start_index, int max_len)
{
  char data[max_len + 1];
  int i;

  for (i = 0; i < max_len; i++) {
    byte b = EEPROM.read(start_index + i);
    if (b == 0) {
      break;  // stop at null terminator
    }
    data[i] = char(b);
  }

  data[i] = '\0'; // terminate at actual stopping point
  return String(data);
}

void load_credentials_EEPROM() {
  ssid     = String(read_EEPROM(ssid_index, 32));
  password = String(read_EEPROM(pass_index, 32));
  
}
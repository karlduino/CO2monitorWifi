// The code needs a private.h file with definitions of
// wifi SSID and password information plus google form information

#define SERIAL_NUMBER "device_serial_number"

// home wifi
#define HOME true   // comment this line out to use enterprise WPA2 (eduroam) instead
#define PRIVATE_SSID "home_wifi_ssid"
#define PRIVATE_PASSWORD "home_wifi_password"

// eduroam (only used if HOME is *not* defined)
#define EAP_USERNAME "username_or_email_for_enterprise_wifi"
#define EAP_PASSWORD "user_password_for_enterprise_wifi"

// google form information
#define PRIVATE_API_CALL "/forms/d/e/SUPER_LONG_GOOGLE_FORM_IDENTIFIER/formResponse?submit=Submit&usp=pp_url"

// cryptic field identifiers that google defines (these examples are random)
#define PRIVATE_ENTRY1 "entry.3619065839"
#define PRIVATE_ENTRY2 "entry.9672122012"
#define PRIVATE_ENTRY3 "entry.9253987371"
#define PRIVATE_ENTRY4 "entry.8270298179"
#define PRIVATE_ENTRY5 "entry.3404483958"

// Double quotes, spaces OK.
#define PLUG_MFR "FX-Team"
#define PLUG_NAME "FX Delay"

// No quotes or spaces.
#define PLUG_CLASS_NAME FX_Delay

// OSX crap.
// - Manually edit the info.plist file to set the CFBundleIdentifier to the either the string 
// "com.BUNDLE_MFR.audiounit.BUNDLE_NAME" or "com.BUNDLE_MFR.vst.BUNDLE_NAME".
// Double quotes, no spaces.
#define BUNDLE_MFR "FXT"
#define BUNDLE_NAME "FX_Delay"

// - Manually create a PLUG_CLASS_NAME.exp file with two entries: _PLUG_ENTRY and _PLUG_VIEW_ENTRY
// (these two defines, each with a leading underscore).
// No quotes or spaces.
#define PLUG_ENTRY FX_Delay_Entry
#define PLUG_VIEW_ENTRY FX_Delay_ViewEntry

// The same strings, with double quotes.  There's no other way, trust me.
#define PLUG_ENTRY_STR "FX_Delay_Entry"
#define PLUG_VIEW_ENTRY_STR "FX_Delay_ViewEntry"

// This is the exported cocoa view class, some hosts display this string.
// No quotes or spaces.
#define VIEW_CLASS FX_Delay_View
#define VIEW_CLASS_STR "FX_Delay_View"

// This is interpreted as 0xMAJR.MN.BG
#define PLUG_VER 0x00013000

// http://service.steinberg.de/databases/plugin.nsf/plugIn?openForm
// 4 chars, single quotes.
#define PLUG_UNIQUE_ID 'FXD'
#define PLUG_MFR_ID 'FXT'

#define PLUG_CHANNEL_IO "1-1 2-2"

#define PLUG_LATENCY 0
#define PLUG_IS_INST 0
#define PLUG_DOES_MIDI 0
#define PLUG_DOES_STATE_CHUNKS 0

// Unique IDs for each image resource.
#define GALKA_ID	101
#define BG_ID		102
#define BUTTON_ID	103
#define LED_ID		104

// Image resource locations for this plug.
#define GALKA_FN	"img/dot.png"
#define BG_FN		"img/bg.png"
#define BUTTON_FN	"img/button.png"
#define LED_FN		"img/led.png"

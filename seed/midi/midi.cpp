#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;
MidiHandler midi;
Logger<> logger;

// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
		case NoteOff:
		{
			NoteOffEvent p = m.AsNoteOff();
			logger.Print("Note Off\n");
			logger.Print("   Note: %d\n", p.note);
			logger.Print("   Velocity: %d:\n", p.velocity);
		}
		break;
		case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
			logger.Print("Note On\n");
			logger.Print("   Note: %d\n", p.note);
			logger.Print("   Velocity: %d:\n", p.velocity);
        }
        break;
		case PolyphonicKeyPressure:
		{
			PolyphonicKeyPressureEvent p = m.AsPolyphonicKeyPressure();
			logger.Print("PKP Event\n");
			logger.Print("   Note: %d\n", p.note);
			logger.Print("   Pressure: %d:\n", p.pressure);
		}
		break;
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
			logger.Print("Control Change\n");
			logger.Print("   Control Number: %d\n", p.control_number);
			logger.Print("   Value: %d:\n", p.value);

        }
		break;
		case ProgramChange:
        {
            ProgramChangeEvent p = m.AsProgramChange();
			logger.Print("Program Change\n");
			logger.Print("   Program Number: %d\n", p.program);
        }
		break;        
		case ChannelPressure:
        {
            ChannelPressureEvent p = m.AsChannelPressure();
			logger.Print("Channel Pressure\n");
			logger.Print("   Pressure: %d\n", p.pressure);
        }
		break;
		case PitchBend:
        {
            PitchBendEvent p = m.AsPitchBend();
			logger.Print("Pitch Bend\n");
			logger.Print("   Value: %d:\n", p.value);
        }
		break;
		case SystemCommon:
        {
			switch(m.sc_type){
				case SystemExclusive:
				{
					SystemExclusiveEvent p = m.AsSystemExclusive();
					logger.Print("SysEx\n");
					logger.Print("    Message Length: %d\n", p.length);
					for(int i = 0; i < p.length; i++){
						logger.Print("    SysEx data: %d\n", p.data[i]);
					}
				}
				break;
				case MTCQuarterFrame:
				{
					MTCQuarterFrameEvent p = m.AsMTCQuarterFrame();
					logger.Print("MTCQuarterFrame\n");
					logger.Print("   Message Type: %d\n", p.message_type);
					logger.Print("   Value: %d\n", p.value);
				}
				break;
				case SongPositionPointer:
				{
					SongPositionPointerEvent p = m.AsSongPositionPointer();
					logger.Print("SongPositionPointer\n");
					logger.Print("   Position: %d\n", p.position);
				}
				break;
				case SongSelect:
				{
					SongSelectEvent p = m.AsSongSelect();
					logger.Print("Song Select\n");
					logger.Print("   Song: %d\n", p.song);

				}
				break;
				case SCUndefined0:
					logger.Print("Undefined0\n");
					break;
				case SCUndefined1:
					logger.Print("Undefined1\n");
					break;
				case TuneRequest:
					logger.Print("TuneRequest\n");
					break;
				case SysExEnd:
					logger.Print("SysExEnd\n");
					break;
				default: break;
			}
        }		
		break;
		case SystemRealTime:
        {
			switch(m.srt_type){
				case TimingClock:
					logger.Print("Timing Clock\n");
					break;
				case SRTUndefined0:
					logger.Print("SRTUndefined0\n");			
					break;			
				case Start:
					logger.Print("Start\n");
					break;
				case Continue:
					logger.Print("Continue\n");
					break;
				case Stop:
					logger.Print("Stop\n");
					break;				
				case SRTUndefined1:
					logger.Print("SRTUndefined1\n");		
					break;			
				case ActiveSensing:
					logger.Print("ActiveSensing\n");
					break;
				case Reset:
					logger.Print("Reset\n");
					break;
				default: break;
			}
        }		
		break;
        default: break;
    }
}

int main(void)
{
	hw.Init();

    midi.Init(MidiHandler::INPUT_MODE_UART1, MidiHandler::OUTPUT_MODE_NONE);
    midi.StartReceive();
	logger.StartLog();
	hw.StartAdc();
	for(;;)
    {
        midi.Listen();
        // Handle MIDI Events
        while(midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }

    }
}
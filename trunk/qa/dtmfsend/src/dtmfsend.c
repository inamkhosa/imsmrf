/******************************************************************************
** Written by Razi-ud-Din Chaudhry
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME        : dtmfsend.c
** DESCRIPTION      : This file acts as a utility for sending the dtmf digits
**			to the given destination IP address and port
** PROJECT NAME     : Media Server
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

#include <gst/gst.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define GST_RTP_DTMF_TYPE_EVENT 1

static GstElement *pipeline, *dtmf_src, *network_sink;
static GMainLoop *main_loop;
static gboolean sending_dtmf;
static char *pcDtmfDigits;
static int nIndx;
static int nDelay;

static struct sigaction sig_action;

int check_digit (char cDigit)	// Generate numeric digit code from given character dtmf digit
{
	if(cDigit >= '0' && cDigit <= '9')
	{
		return atoi(&cDigit);
	}
	else if(cDigit == '*')
	{
		return 10;
	}
	else if(cDigit == '#')
	{
		return 11;
	}
	else if(cDigit == 'A' || cDigit == 'a')
	{
		return 12;
	}
	else if(cDigit == 'B' || cDigit == 'b')
	{
		return 13;
	}
	else if(cDigit == 'C' || cDigit == 'c')
	{
		return 14;
	}
	else if(cDigit == 'D' || cDigit == 'd')
	{
		return 15;
	}
	else
	{
		return -1;
	}
}

static gboolean quit_main_loop(gpointer data)
{
	/* stop main loop */
	g_main_loop_quit (main_loop);

	return FALSE;
}

static void on_dtmf_event (gint dtmf_number, gint volume)
{
	GstEvent *event;
	GstStructure *structure;
// 	GstPad *pad;

	sending_dtmf = !sending_dtmf;

	structure = gst_structure_new ("dtmf-event",
		"type", G_TYPE_INT, GST_RTP_DTMF_TYPE_EVENT,
		"number", G_TYPE_INT, dtmf_number,
		"volume", G_TYPE_INT, volume,
		"start", G_TYPE_BOOLEAN, sending_dtmf, NULL);

	event = gst_event_new_custom (GST_EVENT_CUSTOM_UPSTREAM, structure);
	gst_element_send_event (pipeline, event);
}

static gboolean dtmf_send_time(gpointer data)
{
	/* start sending dtmf digit */
	on_dtmf_event(check_digit(pcDtmfDigits[nIndx]),10);
	g_print ("sending event '%c' with volume '10'..\n", pcDtmfDigits[nIndx]);

	/* stop sending current dtmf digit */
	on_dtmf_event(check_digit(pcDtmfDigits[nIndx]),10);
	g_print ("stopped sending event '%c'.\n", pcDtmfDigits[nIndx]);
	nIndx++;

	if(nIndx >= strlen(pcDtmfDigits))	// all dtmf digits sent
	{	
		/* to wait for the last digit to be received on server */
		g_timeout_add(60, (GSourceFunc)quit_main_loop, NULL);
	}
	else if(nIndx == 1)	// first dtmf digit sent
	{
		/* set the delay for subsequent digits after first dtmf digit has been processed */
		g_timeout_add(nDelay, (GSourceFunc)dtmf_send_time, NULL);
	}
	else	// dtmf digits remaining
	{
		return TRUE;
	}

	return FALSE;
}

static void signal_handler (int sig_num)
{
	switch (sig_num)
	{
		case SIGINT:	// check if user pressed Ctrl + C keys
			if (sending_dtmf)
			{
				/* stop sending current dtmf digit */
				on_dtmf_event (check_digit(pcDtmfDigits[nIndx]), 10);
				g_print ("stopped sending event '%c'.\n", pcDtmfDigits[nIndx]);
			}
			else
			{
				/* sending of current dtmf digit already stopped */
			}

			/* stop main loop */
			g_main_loop_quit (main_loop);
			break;
		default:
			break;
	}
}

static gboolean bus_call (GstBus * bus, GstMessage * msg, gpointer data)
{
	switch (GST_MESSAGE_TYPE (msg))
	{
		case GST_MESSAGE_EOS:
			g_print ("End-of-stream\n");
			g_main_loop_quit (main_loop);
			break;
		case GST_MESSAGE_ERROR:
		{
			gchar *debug;
			GError *err;
			
			gst_message_parse_error (msg, &err, &debug);
			g_free (debug);
			
			g_print ("Error: %s\n", err->message);
			g_error_free (err);
			
			g_main_loop_quit (main_loop);
			break;
		}
		default:
			break;
	}
	
	return TRUE;
}

int main (int argc, char *argv[])
{
	GstBus *bus;
	char *addr = NULL;
	int port = 0;
	int nFirstDelay = 0;

	/* initialize GStreamer */
	gst_init (&argc, &argv);

	if (argc == 2 && strcmp ("--usage", argv[1]) == 0)
	{
		printf("Usage : dtmfsend dest_ip dest_port dtmf_digits delay_before_first_digit_in_ms delay_between_subsequent_digits_in_ms \n");
		return 0;
	}
	else if(argc == 6)
	{
		addr = argv[1];
		port = atoi (argv[2]);	
		pcDtmfDigits = argv[3];
		nFirstDelay = atoi (argv[4]);
		nDelay = atoi (argv[5]);

		/* The utility is not sending all digits if this delay is less than 153 milliseconds */
		if(nDelay < 200)	// Checking if given delay is less than 200 milliseconds
		{
			/* Setting the minimum value of this delay as 200 milliseconds */
			nDelay = 200;
		}
		else
		{
			/* The given delay is greater than or equal to 200 milliseconds */
		}
	}
	else
	{
		printf("Error: invalid number of arguments\n");
		printf("Usage : dtmfsend dest_ip dest_port dtmf_digits delay_before_first_digit_in_ms delay_between_subsequent_digits_in_ms \n");
		return 0;
	}
	

	/* create elements */
	pipeline = gst_pipeline_new ("dtmf-test-pipeline");
	dtmf_src = gst_element_factory_make ("rtpdtmfsrc", "dtmfsrc");
	network_sink = gst_element_factory_make ("udpsink", "networksink");

	if (!pipeline || !dtmf_src || !network_sink)
	{
		g_print ("One element could not be created\n");
		return -1;
	}

	g_object_set (G_OBJECT (dtmf_src), "pt", 101, NULL);

	g_object_set (G_OBJECT (network_sink), "sync", FALSE, NULL);
	if (addr)
	{
		g_object_set (G_OBJECT (network_sink), "host", addr, NULL);
	}
	if (port)
	{
		g_object_set (G_OBJECT (network_sink), "port", port, NULL);
	}

	bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
	gst_bus_add_watch (bus, bus_call, NULL);
	gst_object_unref (bus);

	/* put all elements in a bin */
	gst_bin_add_many (GST_BIN (pipeline), dtmf_src, network_sink, NULL);
	gst_element_link (dtmf_src, network_sink);

	/* Now set to playing and iterate. */
	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	/* set the unix signals */
	bzero(&sig_action, sizeof(sig_action));
	sig_action.sa_handler = signal_handler;
	sigaction(SIGINT, &sig_action, NULL);

	sending_dtmf = FALSE;
	nIndx = 0;

	g_print ("starting pipeline..\n");

	/* set the timer for alarm signal */
	g_timeout_add(nFirstDelay, (GSourceFunc)dtmf_send_time, NULL);

	/*start the main_loop */
	main_loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (main_loop);

	/* clean up nicely */
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (GST_OBJECT (pipeline));
	g_main_loop_unref (main_loop);

	return 0;
}

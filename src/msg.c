/*
 * Copyright (C) 2007  Ignacio Casal Quinteiro <nacho.resa@gmail.com>
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANMsgILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "msg.h"

#include <glib.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gettext-po.h>

#define GTR_MSG_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ( \
					 (object),	\
					 GTR_TYPE_MSG,     \
					 GtranslatorMsgPrivate))


G_DEFINE_TYPE(GtranslatorMsg, gtranslator_msg, G_TYPE_OBJECT)

struct _GtranslatorMsgPrivate
{
	//Missing comment
	//Header too??
	po_message_t message;
};

static gchar *message_error = NULL;

static void
gtranslator_msg_init (GtranslatorMsg *msg)
{
	msg->priv = GTR_MSG_GET_PRIVATE (msg);
}

static void
gtranslator_msg_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtranslator_msg_parent_class)->finalize (object);
}

static void
gtranslator_msg_class_init (GtranslatorMsgClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtranslatorMsgPrivate));

	object_class->finalize = gtranslator_msg_finalize;
}

/***************************** Public funcs ***********************************/

/**
 * gtranslator_msg_new:
 * 
 * Return value: a new #GtranslatorMsg object
 **/
GtranslatorMsg *
gtranslator_msg_new(void)
{
	return g_object_new (GTR_TYPE_MSG, NULL);
}

/**
 * gtranslator_msg_get_message:
 * @msg: a #GtranslatorMsg
 *
 * Return value: the message in gettext format
 **/
po_message_t
gtranslator_msg_get_message(GtranslatorMsg *msg)
{
	return msg->priv->message;
}

/**
 * gtranslator_msg_set_message:
 * @msg: a #GtranslatorMsg
 * @message: the po_message_t to set into the @msg
 *
 * Sets the message into the #GtranslatorMsg class.
 **/
void
gtranslator_msg_set_message(GtranslatorMsg *msg,
			    po_message_t message)
{
	msg->priv->message = message;
}

/**
 * po_message_is_translated:
 * @msg: a #GtranslatorMsg
 * 
 * Return value: TRUE if the message is translated
 **/
gboolean
gtranslator_msg_is_translated (GtranslatorMsg *msg)
{
	if (po_message_msgid_plural(msg->priv->message) == NULL)
		return po_message_msgstr(msg->priv->message)[0] != '\0';	
	else
	{
		gint i;

		for (i = 0; ; i++)
		{
			const gchar *str_i = po_message_msgstr_plural(msg->priv->message, i);
			if (str_i == NULL)
				break;
			if (str_i[0] == '\0')
				return FALSE;
		}
		
		return TRUE;
	}
}

/**
 * gtranslator_msg_is_fuzzy:
 * @msg: a #GtranslatorMsg
 * 
 * Return value: TRUE if the message is fuzzy
 **/
gboolean
gtranslator_msg_is_fuzzy(GtranslatorMsg *msg)
{
	return po_message_is_fuzzy(msg->priv->message);
}


/**
 * gtranslator_msg_set_fuzzy:
 * @msg: a #GtranslatorMsg
 * @fuzzy: the fuzzy value to set to the message
 * 
 * Change the fuzzy mark of a message.
 **/
void
gtranslator_msg_set_fuzzy(GtranslatorMsg *msg,
			  gboolean fuzzy)
{
	po_message_set_fuzzy(msg->priv->message, fuzzy);
}


/**
 * gtranslator_msg_get_msgid:
 * @msg: a #GtranslatorMsg
 *
 * Return value: the msgid (untranslated English string) of a message.
 **/
const gchar *
gtranslator_msg_get_msgid(GtranslatorMsg *msg)
{
	return po_message_msgid(msg->priv->message);
}


/**
 * gtranslator_msg_get_msgid_plural:
 * @msg: a #GtranslatorMsg
 * 
 * Return value: the msgid_plural (untranslated English plural string) of a
 * message, or NULL for a message without plural.
 **/
const gchar *
gtranslator_msg_get_msgid_plural(GtranslatorMsg *msg)
{
	return po_message_msgid_plural(msg->priv->message);	
}


/**
 * gtranslator_msg_get_msgstr:
 * @msg: a #GtranslatorMsg
 * 
 * Return value: the msgstr (translation) of a message.
 * Return the empty string for an untranslated message.
 **/
const gchar *
gtranslator_msg_get_msgstr(GtranslatorMsg *msg)
{
	return po_message_msgstr(msg->priv->message);
}


/**
 * gtranslator_msg_set_msgstr:
 * @msg: a #GtranslatorMsg
 * @msgstr: the string to set in the @msg
 * 
 * Change the msgstr (translation) of a message.
 * Use an empty string to denote an untranslated message.
 **/
void
gtranslator_msg_set_msgstr(GtranslatorMsg *msg,
			   const gchar *msgstr)
{
	po_message_set_msgstr(msg->priv->message, msgstr);
}


/**
 * gtranslator_msg_get_msgstr_plural:
 * @msg: a #GtranslatorMsg
 * @index: the index of the plural array
 *
 * Return value: the msgstr[index] for a message with plural handling, or
 * NULL when the index is out of range or for a message without plural.
 **/
const gchar *
gtranslator_msg_get_msgstr_plural(GtranslatorMsg *msg,
				  gint index)
{
	return po_message_msgstr_plural(msg->priv->message, index);
}

/**
 * gtranslator_msg_set_msgstr_plural:
 * @msg: a #GtranslatorMsg
 * @index: the index where to set the msgstr
 * @msgstr: the message to set in the msg
 *
 * Change the msgstr[index] for a message with plural handling.
 * Use a NULL value at the end to reduce the number of plural forms.
 **/
void
gtranslator_msg_set_msgstr_plural(GtranslatorMsg *msg,
				  gint index,
				  const gchar *msgstr)
{
	po_message_set_msgstr_plural(msg->priv->message,
				     index,
				     msgstr);
}


/**
 * gtranslator_msg_get_comment:
 * @msg: a #GtranslatorMsg
 *
 * Return value: the comments for a message.
 **/
const gchar *
gtranslator_msg_get_comment(GtranslatorMsg *msg)
{
	return po_message_comments(msg->priv->message);
}

/**
 * gtranslator_msg_get_extracted_comments:
 * @msg: a #GtranslatorMsg
 *
 * Return value: the extracted comments for a message.
 **/
const gchar *
gtranslator_msg_get_extracted_comments(GtranslatorMsg *msg)
{
	return po_message_extracted_comments(msg->priv->message);
}


static void
on_gettext_po_xerror(gint severity,
		     po_message_t message,
		     const gchar *filename, size_t lineno, size_t column,
		     gint multiline_p, const gchar *message_text)
{
	message_error = g_strdup(message_text);
}

static void
on_gettext_po_xerror2(gint severity,
		      po_message_t message1,
		      const gchar *filename1, size_t lineno1, size_t column1,
		      gint multiline_p1, const gchar *message_text1,
		      po_message_t message2,
		      const gchar *filename2, size_t lineno2, size_t column2,
		      gint multiline_p2, const gchar *message_text2)
{
	
}


/**
 * gtranslator_msg_check:
 * @msg: a #GtranslatorMsg
 * 
 * Return value: the message error or NULL if there is not any error.
 *
 * Test whether the message translation is a valid format string if the message
 * is marked as being a format string.  
 **/
const gchar *
gtranslator_msg_check(GtranslatorMsg *msg)
{
	struct po_xerror_handler handler;
	handler.xerror = &on_gettext_po_xerror;
	//handler.xerror2 = &on_gettext_po_xerror2;
	
	if(message_error != NULL)
	{
		g_free(message_error);
		message_error = NULL;
	}
	
	po_message_check_format(msg->priv->message, &handler);
	
	if(gtranslator_msg_is_fuzzy(msg) || !gtranslator_msg_is_translated(msg))
		message_error = NULL;
	
	/*Are there any other way to do this?*/
	return message_error;
}
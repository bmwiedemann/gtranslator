/*
 * Copyright (C) 2007  Pablo Sanxiao <psanxiao@gmail.com>
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "header-dialog.h"
#include "header_stuff.h"
#include "utils_gui.h"
#include "prefs-manager.h"
#include "po.h"
#include <glib.h>
#include <glib/gi18n.h>
#include <glib-object.h>
#include <gtk/gtk.h>

#define GTR_HEADER_DIALOG_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ( \
						 	(object),	\
						 	GTR_TYPE_HEADER_DIALOG,     \
						 	GtranslatorHeaderDialogPrivate))

G_DEFINE_TYPE(GtranslatorHeaderDialog, gtranslator_header_dialog, GTK_TYPE_DIALOG)

struct _GtranslatorHeaderDialogPrivate
{
	GtkWidget *main_box;
	GtkWidget *notebook;

	GtkWidget *prj_page;
	GtkWidget *lang_page;
	GtkWidget *lang_vbox;

	GtkWidget *prj_name;
	GtkWidget *prj_version;
	GtkWidget *rmbt;
	GtkWidget *prj_comment;
	GtkWidget *take_my_options;

	GtkWidget *translator;
	GtkWidget *tr_email;
	GtkWidget *pot_date;
	GtkWidget *po_date;
	GtkWidget *language_combo;
	GtkWidget *charset_combo;
	GtkWidget *enc_combo;
	GtkWidget *lg_combo;
};

static void gtranslator_header_dialog_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtranslator_header_dialog_parent_class)->finalize (object);
}

static void gtranslator_header_dialog_class_init (GtranslatorHeaderDialogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtranslatorHeaderDialogPrivate));

	object_class->finalize = gtranslator_header_dialog_finalize;
}

static void
take_my_options_checkbutton_toggled(GtkToggleButton *button,
				  GtranslatorHeaderDialog *dlg)
{
	g_return_if_fail(button == GTK_TOGGLE_BUTTON(dlg->priv->take_my_options));
	
	gtranslator_prefs_manager_set_take_my_options(gtk_toggle_button_get_active(button));
}

static void gtranslator_header_dialog_init (GtranslatorHeaderDialog *dlg)
{
	gboolean ret;
	GtkWidget *error_widget;
	
	dlg->priv = GTR_HEADER_DIALOG_GET_PRIVATE (dlg);

	gtk_dialog_add_buttons (GTK_DIALOG (dlg),
				GTK_STOCK_CLOSE,
				GTK_RESPONSE_CLOSE,
				NULL);
	
	gtk_window_set_title (GTK_WINDOW (dlg), _("gtranslator Edit Header"));
	gtk_window_set_resizable (GTK_WINDOW (dlg), FALSE);
	gtk_dialog_set_has_separator (GTK_DIALOG (dlg), FALSE);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (dlg), TRUE);

	gtk_container_set_border_width (GTK_CONTAINER (dlg), 5);
	gtk_box_set_spacing (GTK_BOX (GTK_DIALOG (dlg)->vbox), 2);
	gtk_container_set_border_width (GTK_CONTAINER (GTK_DIALOG (dlg)->action_area), 5);
	gtk_box_set_spacing (GTK_BOX (GTK_DIALOG (dlg)->action_area), 4);

	g_signal_connect (dlg,
			  "response",
			  G_CALLBACK (gtk_widget_destroy),
			  NULL);
	
	ret = gtranslator_utils_get_glade_widgets(DATADIR"/header-dialog.glade",
		"main_box",
		&error_widget,
		"main_box", &dlg->priv->main_box,
		"notebook", &dlg->priv->notebook,
		"vbox1", &dlg->priv->lang_vbox,
		"prj_name", &dlg->priv->prj_name,
		"prj_version", &dlg->priv->prj_version,
		"rmbt", &dlg->priv->rmbt,
		"prj_comment", &dlg->priv->prj_comment,
		"take_my_options", &dlg->priv->take_my_options,
		"translator", &dlg->priv->translator,
		"tr_email", &dlg->priv->tr_email,
		"pot_date", &dlg->priv->pot_date,
		"po_date", &dlg->priv->po_date,
		"language_combo", &dlg->priv->language_combo,
		"charset_combo", &dlg->priv->charset_combo, 
		"enc_combo", &dlg->priv->enc_combo,
		"lg_combo", &dlg->priv->lg_combo,
		NULL);

	if(!ret)
	{
		gtk_widget_show(error_widget);
		gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dlg)->vbox),
					     error_widget);
		
		return;
	}

	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dlg)->vbox),
			    dlg->priv->main_box, FALSE, FALSE, 0);
	
	gtk_container_set_border_width (GTK_CONTAINER (dlg->priv->notebook), 5);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dlg->priv->take_my_options),
				     gtranslator_prefs_manager_get_take_my_options());

	/*Connect signals*/
	g_signal_connect(dlg->priv->take_my_options, "toggled",
			 G_CALLBACK(take_my_options_checkbutton_toggled),
			 dlg);	

	/*
	 * TODO: Set header's values from GtranslatorHeader object into widget
	 */
}

void gtranslator_show_header_dialog (GtranslatorWindow *window)
{
	
	static GtkWidget *dlg = NULL;

	g_return_if_fail(GTR_IS_WINDOW(window));
	
	if(dlg == NULL)
	{
		dlg = GTK_WIDGET (g_object_new (GTR_TYPE_HEADER_DIALOG, NULL));
		g_signal_connect (dlg,
				  "destroy",
				  G_CALLBACK (gtk_widget_destroyed),
				  &dlg);
		gtk_widget_show_all(dlg);
	}
	
	if (GTK_WINDOW (window) != gtk_window_get_transient_for (GTK_WINDOW (dlg)))
	{
		gtk_window_set_transient_for (GTK_WINDOW (dlg),
					      GTK_WINDOW (window));
	}
	
	gtk_window_present (GTK_WINDOW (dlg));	
}

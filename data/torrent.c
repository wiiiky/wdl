#include <gtk/gtk.h>

GdkPixbuf *get_icon_pixbuf(GtkIconTheme * icon_theme,
						   GThemedIcon * icon, int size)
{
	char **icon_names;
	GtkIconInfo *icon_info;
	GdkPixbuf *pixbuf;
	GError *error = NULL;

	g_object_get(icon, "names", &icon_names, NULL);

	icon_info =
		gtk_icon_theme_choose_icon(icon_theme, (const char **) icon_names,
								   size, 0);
	pixbuf = gtk_icon_info_load_icon(icon_info, &error);
	if (pixbuf == NULL) {
		g_warning("could not load icon pixbuf: %s\n", error->message);
		g_clear_error(&error);
	}

	/*gtk_icon_info_free (icon_info); */
	g_object_unref(icon_info);
	g_strfreev(icon_names);

	return pixbuf;
}


int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkBuilder *ui = gtk_builder_new();
	gtk_builder_add_from_file(ui, "./torrent.glade", NULL);

	GtkWidget *window =
		(GtkWidget *) gtk_builder_get_object(ui, "torrent_window");
	g_signal_connect(G_OBJECT(window), "destroy",
					 G_CALLBACK(gtk_main_quit), NULL);

	GtkTreeModel *model =(GtkTreeModel*) gtk_builder_get_object(ui, "file_tree");
	GdkPixbuf *pixbuf = get_icon_pixbuf(gtk_icon_theme_get_default(),
										(GThemedIcon *)
										g_themed_icon_new
										("gnome-terminal"),
										16);
	GtkTreeIter iter;
	gtk_tree_store_append(GTK_TREE_STORE(model), &iter,NULL);
	gtk_tree_store_set(GTK_TREE_STORE(model), &iter, 0, pixbuf, 
				1,"this is the root folder",
				2,"1.5 GB",3,
				TRUE,-1);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

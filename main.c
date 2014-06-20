#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <curl/curl.h>
#include <string.h>
#include "cJSON.c"

#define max_size 2048

void getwiki(char *text);
char *space_replace(char *str);

struct MemoryStruct {
  char *memory;
  size_t size;
};

struct MemoryStruct chunk;

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


/**To remove first and last charactors because they are quotes**/
void mod_txt(const char *input, char *output)
{
  int len = strlen(input);
  if(len > 0)
    strcpy(output, ++input);
  if(len > 1)
    output[len - 2] = '\0';
}


/** Parse text to JSON, then render back to text, and print! **/
char *parse(char *text)
{
    int i;
	char *out;
	char nw_txt[max_size];
	char final_txt[max_size]= "";
	cJSON* para = NULL;
	cJSON *json;

	json=cJSON_Parse(text);
	 cJSON *item = cJSON_GetObjectItem(json,"query");
	   printf("%s\n\n",text);


  for (i = 0 ; i < cJSON_GetArraySize(item) ; i++)
  {
     cJSON * subitem = cJSON_GetArrayItem(item, i);
     para = cJSON_GetObjectItem(subitem, "extract");
     out = cJSON_Print(para);
     mod_txt(out,nw_txt);

    sprintf(final_txt, "%s%s", final_txt, nw_txt);
  }
  printf("\n%s\n", final_txt);
  //return final_txt;
}



int main(int argc, char *argv[])
{
     gtk_init(&argc, &argv);

         GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_default_size(GTK_WINDOW(window), 700, 580);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        GtkWidget* fixed_container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed_container);

    GtkWidget *button = gtk_button_new_with_label("Get");
     gtk_fixed_put(GTK_FIXED(fixed_container), button, 655, 10);

     GtkWidget *entry1 = gtk_entry_new();
      gtk_fixed_put(GTK_FIXED(fixed_container), entry1, 10, 10);
      gtk_widget_set_size_request(entry1, 645, 15);
    gtk_entry_set_text(GTK_ENTRY(entry1),"Paul Walker");

         GtkWidget *text1 = gtk_text_view_new();
      gtk_fixed_put(GTK_FIXED(fixed_container), text1, 10, 50);
          //gtk_container_add(GTK_CONTAINER(scrolledwindow), text1);
      gtk_widget_set_size_request(text1, 675, 520);
          gtk_text_view_set_wrap_mode(text1,GTK_WRAP_WORD_CHAR);

 void process()
{
    getwiki(gtk_entry_get_text(GTK_ENTRY(entry1)));

    parse(chunk.memory);
}

  g_signal_connect(button, "clicked",
      G_CALLBACK(process), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void getwiki(char *text)
{
    CURL *curl;
    char url[max_size];
    char *text_mod = space_replace(text);
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    sprintf(url,"https://en.wikipedia.org/w/api.php?action=query&prop=extracts&explaintext&titles=%s&format=json&exintro", text_mod);

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); /**write downloaded data directly to the memory**/
        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_global_cleanup();

}

char *space_replace(char *str)
{
        int l = strlen(str);
        int i = 0, j =0;
        int spc = 0, nl = 0;
        char *str1 = NULL;

        for(i = 0; i < l; i++)  {
                if(str[i] == ' ') {
                        spc++;
                }
        }
        nl = l + 2*spc + 1;
        str1 = (char *) malloc(sizeof(char) * nl);
        if(!str1) {
                fprintf(stdout, "malloc failed \n");
                return NULL;
        }
        for(i = 0; i < l; i++) {
                if(str[i] == ' ') {
                        str1[j++] = '%';
                        str1[j++] = '2';
                        str1[j++] = '0';
                } else {
                        str1[j++] =  str[i];
                }
        }
        str1[j] = '\0';
        return str1;
        free(str1);

}

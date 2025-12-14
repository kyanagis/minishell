#include "expander.h"

void init_work_buf(t_expand_work_buf *buf)
{
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}

void destroy_work_buf(t_expand_work_buf *buf)
{
	if (buf->data)
		free(buf->data);
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}

static bool buf_reserve(t_expand_work_buf *buf, size_t add_len)
{
	size_t need_cap;
	size_t new_cap;
	char *new_data;

	need_cap = buf->len + add_len + 1;
	if (need_cap <= buf->cap)
		return (true);
	new_cap = buf->cap;
	if (new_cap == 0)
		new_cap = 32;
	while (new_cap < need_cap)
		new_cap *= 2;
	new_data = (char *)malloc(new_cap);
	if (!new_data)
		return (false);
	if (buf->data && buf->len > 0)
		ft_memcpy(new_data, buf->data, buf->len);
	free(buf->data);
	buf->data = new_data;
	buf->cap = new_cap;
	buf->data[buf->len] = '\0';
	return (true);
}

bool buf_append_char(t_expand_work_buf *buf, char c)
{
	if (!buf_reserve(buf, 1))
		return (false);
	buf->data[buf->len] = c;
	buf->len++;
	buf->data[buf->len] = '\0';
	return (true);
}

bool buf_append_str(t_expand_work_buf *buf, const char *s)
{
	size_t len;

	if (!s)
		return (true);
	len = ft_strlen(s);
	if (!buf_reserve(buf, len))
		return (false);
	ft_memcpy(buf->data + buf->len, s, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (true);
}

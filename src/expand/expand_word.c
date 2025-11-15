
#include "expander.h"

static void	init_work_buf(t_expand_work_buf *buf)
{
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}

static void	destroy_work_buf(t_expand_work_buf *buf)
{
	if (buf->data)
		free(buf->data);
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}

static bool	buf_reserve(t_expand_work_buf *buf, size_t add_len)
{
	size_t	new_cap;
	char	*new_data;
	size_t	min_cap;

	if (buf->len + add_len + 1 <= buf->cap)
		return (true);
	min_cap = buf->len + add_len + 1;
	if (buf->cap == 0)
		new_cap = 16;
	else
		new_cap = buf->cap * 2;
	while (new_cap < min_cap)
		new_cap *= 2;
	new_data = (char *)malloc(new_cap);
	if (!new_data)
		return (false);
	if (buf->data && buf->len > 0)
		ft_memcpy(new_data, buf->data, buf->len);
	new_data[buf->len] = '\0';
	free(buf->data);
	buf->data = new_data;
	buf->cap = new_cap;
	return (true);
}

static bool	buf_append_char(t_expand_work_buf *buf, char c)
{
	if (!buf_reserve(buf, 1))
		return (false);
	buf->data[buf->len] = c;
	buf->len++;
	buf->data[buf->len] = '\0';
	return (true);
}
static bool	buf_append_str(t_expand_work_buf *buf, const char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if (!buf_reserve(buf, len))
		return (false);
	ft_memcpy(buf->data + buf->len, s, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (true);
}

bool	expand_word(t_shell *sh, const char *src, const unsigned char *mask,
		size_t len, char **out)
{
	size_t				i;
	char				c;
	unsigned char		tmp_mask;
	t_expand_work_buf	buf;
	char				*tmp;

	init_work_buf(&buf);
	i = 0;
	while (i < len)
	{
		c = src[i];
		if (mask)
			tmp_mask = mask[i];
		else
			tmp_mask = QM_NONE;
		if (c != '$' || tmp_mask == QM_SQ)
		{
			if (!buf_append_char(&buf, c))
			{
				destroy_work_buf(&buf);
				return (false);
			}
			++i;
			continue ;
		}
		//ここから c == '$' && SQ 以外
		if (i + 1 >= len)
		{
			if (!buf_append_char(&buf, '$'))
			{
				destroy_work_buf(&buf);
				return (false);
			}
			++i;
			continue ;
		}
		if (src[i + 1] == '?')
		{
			tmp = ft_itoa(sh->last_status);
			if (!tmp)
			{
				destroy_work_buf(&buf);
				return (false);
			}
			if (!buf_append_str(&buf, tmp))
			{
				free(tmp);
				destroy_work_buf(&buf);
				return (false);
			}
			free(tmp);
			i += 2;
			continue ;
		}
		// TODO: ここで $VAR の処理を入れる */
		// とりあえず適当にリテラル '$' として扱う */
		if (!buf_append_char(&buf, '$'))
		{
			destroy_work_buf(&buf);
			return (false);
		}
		++i;
	}
	// work_buf の所有権を out に渡す */
	if (!buf.data)
	{
		*out = ft_strdup("");
		if (!*out)
			return (false);
		return (true);
	}
	*out = buf.data;
	// buf.data の free は呼び出し側（cmd / redir 側）に任せる */
	return (true);
}

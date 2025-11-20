#include "expander.h"

static void init_work_buf(t_expand_work_buf *buf)
{
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}

static void destroy_work_buf(t_expand_work_buf *buf)
{
	if (buf->data)
		free(buf->data);
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}

static bool buf_reserve(t_expand_work_buf *buf, size_t add_len)
{
	char *new_data;
	size_t new_cap;
	size_t need_cap;

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

static bool buf_append_char(t_expand_work_buf *buf, char c)
{
	if (!buf_reserve(buf, 1))
		return (false);
	buf->data[buf->len] = c;
	buf->len++;
	buf->data[buf->len] = '\0';
	return (true);
}

static bool buf_append_str(t_expand_work_buf *buf, const char *s)
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

static const char *lookup_env(t_shell *sh, const char *key, size_t len)
{
	size_t idx;

	if (!sh || !sh->envp || len == 0)
		return (NULL);
	idx = 0;
	while (sh->envp[idx])
	{
		if (ft_strncmp(sh->envp[idx], key, len) == 0 && sh->envp[idx][len] == '=')
			return (sh->envp[idx] + len + 1);
		++idx;
	}
	return (NULL);
}

static bool append_last_status(t_expand_work_buf *buf, t_shell *sh)
{
	char *value;

	value = ft_itoa(sh->last_status);
	if (!value)
		return (false);
	if (!buf_append_str(buf, value))
	{
		free(value);
		return (false);
	}
	free(value);
	return (true);
}

static size_t read_var_len(const char *src)
{
	size_t len;

	len = 0;
	while (src[len])
	{
		if (!ft_isalnum((unsigned char)src[len]) && src[len] != '_')
			break;
		++len;
	}
	return (len);
}

static bool append_variable(t_expand_work_buf *buf, t_shell *sh,
							const char *src, size_t len)
{
	const char *value;

	value = lookup_env(sh, src, len);
	if (!value)
		return (true);
	return (buf_append_str(buf, value));
}

static bool handle_dollar(t_expand_work_buf *buf, t_shell *sh,
						  const char *src, size_t *idx, size_t len)
{
	size_t var_len;

	if (*idx + 1 >= len)
		return (buf_append_char(buf, src[(*idx)++]));
	if (src[*idx + 1] == '?')
	{
		*idx += 2;
		return (append_last_status(buf, sh));
	}
	var_len = read_var_len(src + *idx + 1);
	if (var_len == 0)
		return (buf_append_char(buf, src[(*idx)++]));
	if (!append_variable(buf, sh, src + *idx + 1, var_len))
		return (false);
	*idx += var_len + 1;
	return (true);
}

static unsigned char read_mask(const unsigned char *mask, size_t idx)
{
	if (!mask)
		return (QM_NONE);
	return (mask[idx]);
}

static bool is_pure_single_quote(const unsigned char *mask, size_t len)
{
	size_t idx;

	if (!mask)
		return (false);
	idx = 0;
	while (idx < len)
	{
		if (mask[idx] != QM_SQ)
			return (false);
		++idx;
	}
	return (true);
}

bool expand_word(t_shell *sh, const char *src, const unsigned char *mask,
				 size_t len, char **out)
{
	size_t i;
	t_expand_work_buf buf;

	if (!src || !out)
		return (false);
	if (is_pure_single_quote(mask, len))
	{
		*out = ft_strdup(src);
		if (!*out)
			return (false);
		return (true);
	}
	init_work_buf(&buf);
	i = 0;
	while (i < len)
	{
		if (src[i] == '$' && read_mask(mask, i) != QM_SQ)
		{
			if (!handle_dollar(&buf, sh, src, &i, len))
			{
				destroy_work_buf(&buf);
				return (false);
			}
			continue;
		}
		if (!buf_append_char(&buf, src[i]))
		{
			destroy_work_buf(&buf);
			return (false);
		}
		++i;
	}
	if (!buf.data)
	{
		*out = ft_strdup("");
		if (!*out)
			return (false);
		return (true);
	}
	*out = buf.data;
	return (true);
}

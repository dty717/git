#ifndef APPLY_H
#define APPLY_H

enum apply_ws_error_action {
	nowarn_ws_error,
	warn_on_ws_error,
	die_on_ws_error,
	correct_ws_error
};

enum apply_ws_ignore {
	ignore_ws_none,
	ignore_ws_change
};

/*
 * We need to keep track of how symlinks in the preimage are
 * manipulated by the patches.  A patch to add a/b/c where a/b
 * is a symlink should not be allowed to affect the directory
 * the symlink points at, but if the same patch removes a/b,
 * it is perfectly fine, as the patch removes a/b to make room
 * to create a directory a/b so that a/b/c can be created.
 *
 * See also "struct string_list symlink_changes" in "struct
 * apply_state".
 */
#define APPLY_SYMLINK_GOES_AWAY 01
#define APPLY_SYMLINK_IN_RESULT 02

struct apply_state {
	const char *prefix;
	int prefix_length;

	/* These are lock_file related */
	struct lock_file *lock_file;
	int newfd;

	/* These control what gets looked at and modified */
	int apply; /* this is not a dry-run */
	int cached; /* apply to the index only */
	int check; /* preimage must match working tree, don't actually apply */
	int check_index; /* preimage must match the indexed version */
	int update_index; /* check_index && apply */

	/* These control cosmetic aspect of the output */
	int diffstat; /* just show a diffstat, and don't actually apply */
	int numstat; /* just show a numeric diffstat, and don't actually apply */
	int summary; /* just report creation, deletion, etc, and don't actually apply */

	/* These boolean parameters control how the apply is done */
	int allow_overlap;
	int apply_in_reverse;
	int apply_with_reject;
	int apply_verbosely;
	int no_add;
	int threeway;
	int unidiff_zero;
	int unsafe_paths;

	/* Other non boolean parameters */
	const char *fake_ancestor;
	const char *patch_input_file;
	int line_termination;
	struct strbuf root;
	int p_value;
	int p_value_known;
	unsigned int p_context;

	/* Exclude and include path parameters */
	struct string_list limit_by_name;
	int has_include;

	/* Various "current state" */
	int linenr; /* current line number */
	struct string_list symlink_changes; /* we have to track symlinks */

	/*
	 * For "diff-stat" like behaviour, we keep track of the biggest change
	 * we've seen, and the longest filename. That allows us to do simple
	 * scaling.
	 */
	int max_change;
	int max_len;

	/*
	 * Records filenames that have been touched, in order to handle
	 * the case where more than one patches touch the same file.
	 */
	struct string_list fn_table;

	/* These control whitespace errors */
	enum apply_ws_error_action ws_error_action;
	enum apply_ws_ignore ws_ignore_action;
	const char *whitespace_option;
	int whitespace_error;
	int squelch_whitespace_errors;
	int applied_after_fixing_ws;
};

extern int parse_whitespace_option(struct apply_state *state,
				   const char *option);
extern int parse_ignorewhitespace_option(struct apply_state *state,
					 const char *option);

extern void init_apply_state(struct apply_state *state,
			     const char *prefix,
			     struct lock_file *lock_file);
extern void clear_apply_state(struct apply_state *state);

#endif

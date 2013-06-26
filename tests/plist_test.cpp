#define BOOST_TEST_MODULE plist_test

#include "plib.h"

#include <string.h>

#include <boost/test/included/unit_test.hpp>

typedef struct _TestData {
	pint test_array[3];
	pint index;
} TestData;

static void foreach_test_func (ppointer data, ppointer user_data)
{
	if (user_data == NULL)
		return;

	TestData *test_data = (TestData *) user_data;

	if (test_data->index < 0 || test_data->index > 2)
		return;

	test_data->test_array[test_data->index] = P_POINTER_TO_INT (data);
	++test_data->index;
}

BOOST_AUTO_TEST_SUITE (BOOST_TEST_MODULE)

BOOST_AUTO_TEST_CASE (plist_general_test)
{
	PList		*list = NULL;
	TestData	test_data;

	/* Testing append */
	list = p_list_append (list, P_INT_TO_POINTER (32));
	list = p_list_append (list, P_INT_TO_POINTER (64));

	BOOST_REQUIRE (list != NULL);
	BOOST_CHECK (p_list_length (list) == 2);
	
	/* Testing data access */
	BOOST_CHECK (P_POINTER_TO_INT (list->data) == 32);
	BOOST_CHECK (P_POINTER_TO_INT (p_list_last(list)->data) == 64);

	/* Testing prepend */
	list = p_list_prepend (list, P_INT_TO_POINTER (128));
	BOOST_REQUIRE (list != NULL);
	BOOST_CHECK (p_list_length (list) == 3);
	BOOST_CHECK (P_POINTER_TO_INT (list->data) == 128);
	BOOST_CHECK (P_POINTER_TO_INT (p_list_last(list)->data) == 64);

	/* Testing for each loop */
	memset (&test_data, 0, sizeof (test_data));

	BOOST_REQUIRE (test_data.test_array[0] == 0);
	BOOST_REQUIRE (test_data.test_array[1] == 0);
	BOOST_REQUIRE (test_data.test_array[2] == 0);
	BOOST_REQUIRE (test_data.index == 0);

	p_list_foreach (list, (PFunc) foreach_test_func, (ppointer) &test_data);

	BOOST_CHECK (test_data.index == 3);
	BOOST_CHECK (test_data.test_array[0] == 128);
	BOOST_CHECK (test_data.test_array[1] == 32);
	BOOST_CHECK (test_data.test_array[2] == 64);

	/* Testing remove */
	list = p_list_remove (list, P_INT_TO_POINTER (32));
	BOOST_REQUIRE (list != NULL);
	BOOST_CHECK (p_list_length (list) == 2);

	list = p_list_remove (list, P_INT_TO_POINTER (128));
	BOOST_REQUIRE (list != NULL);
	BOOST_CHECK (p_list_length (list) == 1);

	list = p_list_remove (list, P_INT_TO_POINTER (256));
	BOOST_REQUIRE (list != NULL);
	BOOST_CHECK (p_list_length (list) == 1);

	list = p_list_remove (list, P_INT_TO_POINTER (64));
	BOOST_REQUIRE (list == NULL);
	BOOST_CHECK (p_list_length (list) == 0);

	p_list_free (list);
}

BOOST_AUTO_TEST_SUITE_END()

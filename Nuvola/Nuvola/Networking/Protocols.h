#pragma once

namespace Protocol
{
	const int SIGN_IN_REQUEST = 200;
	const int SIGN_IN_SUCCESS = 1000;
	const int SIGN_IN_WRONG_DETAILS = 1001;
	const int SIGN_IN_USER_ALREADY_CONNECTED = 1002;
	const int SIGN_IN_DETAILS_NOT_VALID = 1003;

	const int SIGN_OUT_REQUEST = 201;

	const int SIGN_UP_REQUEST = 202;
	const int SIGN_UP_SUCCESS = 1010;
	const int SIGN_UP_PASS_ILLEGAL = 1011;
	const int SIGN_UP_USERNAME_EXISTS = 1012;
	const int SIGN_UP_USERNAME_ILLEGAL = 1013;
	const int SIGN_UP_OTHER_ERROR = 1014;
	const int SIGN_UP_NOT_ENOUGH_ROOM = 1015;

	const int UPLOAD_FILE_REQUEST = 203;
	const int GET_FILE = 204;
	const int GET_ALL_FILES_INFO = 213;
	const int GET_YOUR_FILE_BACK = 214;

	const int GET_USER_INFO_REQUEST = 205;
	const int GET_ALL_USERS = 210;

	const int CREATE_NEW_GROUP = 206;
	const int ADD_USER_TO_GROUP = 207;
	const int GET_INFO_ABOUT_GROUPS = 208;
	const int UPLOAD_FILE_TO_GROUP = 209;
	const int DELETE_USER_FROM_GROUP = 212;

	const int GET_WORK_END = 211;
}
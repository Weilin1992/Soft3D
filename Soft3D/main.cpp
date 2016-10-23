#include <Windows.h>
#include <tchar.h>
#include "Device.h"
int screen_w, screen_h, screen_exit = 0;
int  screen_mx = 0, screen_my = 0, screen_mb = 0;

int screen_keys[512];
static HWND screen_handle = NULL;
static HDC screen_dc = NULL;
static HBITMAP screen_hb = NULL;
static HBITMAP screen_ob = NULL;
IUINT32 *screen_fb = NULL;

long screen_pitch = 0;
int screen_init(int w, int h, const TCHAR *title);
int screen_close(void);
void screen_dispatch(void);
void screen_update(void);
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

int screen_init(int w, int h, const TCHAR *title) {
	WNDCLASS wc = { CS_BYTEALIGNCLIENT,(WNDPROC)screen_events,0,0,0,NULL,NULL,NULL,NULL,("SCREENPI") };
	BITMAPINFO bi = { {sizeof(BITMAPINFOHEADER),w,-h,1,32,BI_RGB,w*h * 4,0,0,0,0} };
	RECT rect = { 0,0,w,h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	screen_handle = CreateWindow(("SCREENPI"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return -2;
	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);
	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (IUINT32*)ptr;
	screen_w = w;
	screen_h = h;
	screen_pitch = w * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);
	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0, w * h * sizeof(IUINT32));

	return 0;
}


int screen_close(void) {
	if (screen_dc) {
		if (screen_ob) {
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void screen_update(void) {
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	screen_dispatch();
}

int main(void)
{

	int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME,RENDER_STATE_LIGHT };
	int indicator = 0;
	int kbhit = 0;
	float alpha = 1;
	float pos = 3.0f;

	TCHAR *title = _T("Mini3d (software render tutorial) - ")
		_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(800, 600, title))
		return -1;

	Device device(800, 600, screen_fb);
	
	PointLight l = { { -100.f, 100.f, 100.f, 1.f },{ 0.5f, 0.5f, 0.5f }, {0.1,0.1,0.1} };
	ParallelLight PL = { { -1.f, 1.f, -1.f, 0.f },{ 0.5f, 0.5f, 0.5f } };
	device.pointLight = &l;
	device.paraLight = &PL;
	device.init_texture();
	device.render_state = RENDER_STATE_TEXTURE;


	/*
	int vsize, fsize;
	Vertex* pVertexs;
	Face* pFaces;
	*/
	//LoadMesh("models/teapot.obj", pVertexs, vsize, pFaces, fsize);

	device.load_model("models/teapot.obj");

	float theta = 0.0;
	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		screen_dispatch();
		device.clear(1);
		device.camera_at_zero(pos, 3, 3);
		if (screen_keys[VK_UP]) pos -= 0.01f;
		if (screen_keys[VK_DOWN]) pos += 0.01f;
		if (screen_keys[VK_LEFT]) alpha += 0.01f;
		if (screen_keys[VK_RIGHT]) alpha -= 0.01f;
		//device.transform_Plight(theta+=0.1);

		if (screen_keys[VK_SPACE]) {
			if (kbhit == 0) {
				kbhit = 1;
				if (++indicator >= 3) indicator = 0;
				device.render_state = states[indicator];
			}
		}
		else {
			kbhit = 0;
		}
		device.draw_model(alpha);
		//device.draw_box(alpha);
		screen_update();
		Sleep(1);
	}
	return 0;
}


#include "stdafx.h"
#include "resource.h"
#include "VideoOptionsDialog.h"

#define GET_WINDOW_MODES		WM_USER+100
#define GET_FULLSCREENMODES		WM_USER+101

VideoOptionsDialog::VideoOptionsDialog() : CDialog(IDD_VIDEO_OPTIONS)
{
}

void VideoOptionsDialog::AddToolTip(const char * const text, HWND parentHwnd, HWND toolTipHwnd, HWND controlHwnd)
{
   TOOLINFO toolInfo = { 0 };
   toolInfo.cbSize = sizeof(toolInfo);
   toolInfo.hwnd = parentHwnd;
   toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
   toolInfo.uId = (UINT_PTR)controlHwnd;
   toolInfo.lpszText = (char*)text;
   SendMessage(toolTipHwnd, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
}

void VideoOptionsDialog::ResetVideoPreferences(const unsigned int profile) // 0 = default, 1 = lowend PC, 2 = highend PC
{
   if(profile == 0)
   {
   const bool fullscreen = IsWindows10_1803orAbove();
   SendMessage(GetDlgItem(IDC_FULLSCREEN).GetHwnd(), BM_SETCHECK, fullscreen ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetHwnd(), fullscreen ? GET_FULLSCREENMODES : GET_WINDOW_MODES, 0, 0);
   }
   if(profile < 2)
   {
   SendMessage(GetDlgItem(IDC_10BIT_VIDEO).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   }

   SendMessage(GetDlgItem(IDC_Tex3072).GetHwnd(), BM_SETCHECK, BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_Tex1024).GetHwnd(), BM_SETCHECK, BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_Tex2048).GetHwnd(), BM_SETCHECK, profile == 1 ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_TexUnlimited).GetHwnd(), BM_SETCHECK, profile != 1 ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_GLOBAL_REFLECTION_CHECK).GetHwnd(), BM_SETCHECK, profile != 1 ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_GLOBAL_TRAIL_CHECK).GetHwnd(), BM_SETCHECK, true ? BST_CHECKED : BST_UNCHECKED, 0);
   SetDlgItemInt(IDC_ADAPTIVE_VSYNC, 0, FALSE);
   SetDlgItemInt(IDC_MAX_PRE_FRAMES, 0, FALSE);

   if(profile == 0)
   {
   constexpr float ballAspecRatioOffsetX = 0.0f;
   char tmp[256];
   sprintf_s(tmp, 256, "%f", ballAspecRatioOffsetX);
   SetDlgItemTextA(IDC_CORRECTION_X, tmp);
   constexpr float ballAspecRatioOffsetY = 0.0f;
   sprintf_s(tmp, 256, "%f", ballAspecRatioOffsetY);
   SetDlgItemTextA(IDC_CORRECTION_Y, tmp);
   constexpr float latitude = 52.52f;
   sprintf_s(tmp, 256, "%f", latitude);
   SetDlgItemTextA(IDC_DN_LATITUDE, tmp);
   constexpr float longitude = 13.37f;
   sprintf_s(tmp, 256, "%f", longitude);
   SetDlgItemTextA(IDC_DN_LONGITUDE, tmp);
   constexpr float nudgeStrength = 2e-2f;
   sprintf_s(tmp, 256, "%f", nudgeStrength);
   SetDlgItemTextA(IDC_NUDGE_STRENGTH, tmp);
   }

   SendMessage(GetDlgItem(IDC_AA_ALL_TABLES).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_DYNAMIC_DN).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_DYNAMIC_AO).GetHwnd(), BM_SETCHECK, profile == 2 ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_ENABLE_AO).GetHwnd(), BM_SETCHECK, true ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_GLOBAL_SSREFLECTION_CHECK).GetHwnd(), BM_SETCHECK, profile == 2 ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_GLOBAL_PFREFLECTION_CHECK).GetHwnd(), BM_SETCHECK, profile != 1 ? BST_CHECKED : BST_UNCHECKED, 0);

   if (profile == 0)
   {
   SendMessage(GetDlgItem(IDC_OVERWRITE_BALL_IMAGE_CHECK).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   SetDlgItemText(IDC_BALL_IMAGE_EDIT, "");
   SetDlgItemText(IDC_BALL_DECAL_EDIT, "");
   if (true)
   {
      ::EnableWindow(GetDlgItem(IDC_BROWSE_BALL_IMAGE).GetHwnd(), FALSE);
      ::EnableWindow(GetDlgItem(IDC_BROWSE_BALL_DECAL).GetHwnd(), FALSE);
      ::EnableWindow(GetDlgItem(IDC_BALL_IMAGE_EDIT).GetHwnd(), FALSE);
      ::EnableWindow(GetDlgItem(IDC_BALL_DECAL_EDIT).GetHwnd(), FALSE);
   }
   }

   SendMessage(GetDlgItem(IDC_FXAACB).GetHwnd(), CB_SETCURSEL, profile == 1 ? Disabled : (profile == 2 ? Quality_FXAA : Standard_FXAA), 0);
   SendMessage(GetDlgItem(IDC_SHARPENCB).GetHwnd(), CB_SETCURSEL, profile != 2 ? 0 : 2, 0);
   SendMessage(GetDlgItem(IDC_SCALE_FX_DMD).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);

   if (profile == 0)
   {
   SendMessage(GetDlgItem(IDC_BG_SET).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_3D_STEREO).GetHwnd(), CB_SETCURSEL, 0, 0);
   SendMessage(GetDlgItem(IDC_3D_STEREO_Y).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   constexpr float stereo3DOfs = 0.0f;
   char tmp[256];
   sprintf_s(tmp, 256, "%f", stereo3DOfs);
   SetDlgItemTextA(IDC_3D_STEREO_OFS, tmp);
   constexpr float stereo3DMS = 0.03f;
   sprintf_s(tmp, 256, "%f", stereo3DMS);
   SetDlgItemTextA(IDC_3D_STEREO_MS, tmp);
   constexpr float stereo3DZPD = 0.5f;
   sprintf_s(tmp, 256, "%f", stereo3DZPD);
   SetDlgItemTextA(IDC_3D_STEREO_ZPD, tmp);
   constexpr float stereo3DContrast = 1.0f;
   sprintf_s(tmp, 256, "%f", stereo3DContrast);
   SetDlgItemTextA(IDC_3D_STEREO_CONTRAST, tmp);
   constexpr float stereo3DDeSaturation = 0.0f;
   sprintf_s(tmp, 256, "%f", stereo3DDeSaturation);
   SetDlgItemTextA(IDC_3D_STEREO_DESATURATION, tmp);
   SendMessage(GetDlgItem(IDC_USE_NVIDIA_API_CHECK).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   }

   SendMessage(GetDlgItem(IDC_BLOOM_OFF).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);

   if (profile == 0)
   {
   SendMessage(GetDlgItem(IDC_DISABLE_DWM).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   }

   SendMessage(GetDlgItem(IDC_FORCE_ANISO).GetHwnd(), BM_SETCHECK, profile != 1 ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_TEX_COMPRESS).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_SOFTWARE_VP).GetHwnd(), BM_SETCHECK, false ? BST_CHECKED : BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_ARASlider).GetHwnd(), TBM_SETPOS, TRUE, profile == 1 ? 5 : 10);

   if (profile == 0)
   {
   SendMessage(GetDlgItem(IDC_StretchYes).GetHwnd(), BM_SETCHECK, BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_StretchMonitor).GetHwnd(), BM_SETCHECK, BST_UNCHECKED, 0);
   SendMessage(GetDlgItem(IDC_StretchNo).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0);
   //SendMessage(GetDlgItem(IDC_MonitorCombo).GetHwnd(), CB_SETCURSEL, 1, 0);
   SendMessage(GetDlgItem(IDC_DISPLAY_ID).GetHwnd(), CB_SETCURSEL, 0, 0);
   }
}

void VideoOptionsDialog::FillVideoModesList(const std::vector<VideoMode>& modes, const VideoMode* curSelMode)
{
   const HWND hwndList = GetDlgItem(IDC_SIZELIST).GetHwnd();
   SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
   SendMessage(hwndList, LB_INITSTORAGE, modes.size(), modes.size() * 128); // *128 is artificial

   int bestMatch = 0; // to find closest matching res
   int bestMatchingPoints = 0; // dto.

   int screenwidth, screenheight;
   int x, y;
   const int display = (int)SendMessage(GetDlgItem(IDC_DISPLAY_ID).GetHwnd(), CB_GETCURSEL, 0, 0);
   getDisplaySetupByID(display, x, y, screenwidth, screenheight);

   for (size_t i = 0; i < modes.size(); ++i)
   {
      char szT[128];

      double aspect = (double)modes[i].width / (double)modes[i].height;
      const bool portrait = (aspect < 1.);
      if (portrait)
         aspect = 1./aspect;
      double factor = aspect*3.0;
      int fx,fy;
      if (factor > 4.0)
      {
         factor = aspect*9.0;
         if ((int)(factor+0.5) == 16)
         {
            //16:9
            fx = 16;
            fy = 9;
         }
         else if ((int)(factor+0.5) == 21)
         {
            //21:9
            fx = 21;
            fy = 9;
         }
         else
         {
            factor = aspect*10.0;
            if ((int)(factor+0.5) == 16)
            {
               //16:10
               fx = 16;
               fy = 10;
            }
            else
            {
               //21:10
               fx = 21;
               fy = 10;
            }
         }
      }
      else
      {
         //4:3
         fx = 4;
         fy = 3;
      }

      if (modes[i].depth) // i.e. is this windowed or not
         sprintf_s(szT, "%d x %d (%dHz %d:%d)", modes[i].width, modes[i].height, /*modes[i].depth,*/ modes[i].refreshrate, portrait ? fy : fx, portrait ? fx : fy);
      else
         sprintf_s(szT, "%d x %d (%d:%d %s)", modes[i].width, modes[i].height /*,modes[i].depth*/, portrait ? fy : fx, portrait ? fx : fy, portrait ? "Portrait" : "Landscape");

      SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)szT);
      if (curSelMode) {
         int matchingPoints = 0;
         if (modes[i].width == curSelMode->width) matchingPoints += 100;
         if (modes[i].height == curSelMode->height) matchingPoints += 100;
         if (modes[i].depth == curSelMode->depth) matchingPoints += 50;
         if (modes[i].refreshrate == curSelMode->refreshrate) matchingPoints += 10;
         if (modes[i].width == screenwidth) matchingPoints += 3;
         if (modes[i].height == screenheight) matchingPoints += 3;
         if (modes[i].refreshrate == DEFAULT_PLAYER_FS_REFRESHRATE) matchingPoints += 1;
         if (matchingPoints > bestMatchingPoints) {
            bestMatch = (int)i;
            bestMatchingPoints = matchingPoints;
         }
      }
   }
   SendMessage(hwndList, LB_SETCURSEL, bestMatch, 0);
}


BOOL VideoOptionsDialog::OnInitDialog()
{
   const HWND hwndDlg = GetHwnd();
   const HWND toolTipHwnd = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwndDlg, NULL, g_pvp->theInstance, NULL);
   if (toolTipHwnd)
   {
      SendMessage(toolTipHwnd, TTM_SETMAXTIPWIDTH, 0, 180);
      AddToolTip("Activate this if you get the corresponding error message on table start, or if you experience rendering problems.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_USE_NVIDIA_API_CHECK).GetHwnd());
      AddToolTip("Forces the bloom filter to be always off. Only for very low-end graphics cards.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_BLOOM_OFF).GetHwnd());
      AddToolTip("This saves memory on your graphics card but harms quality of the textures.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_TEX_COMPRESS).GetHwnd());
      AddToolTip("Disable Windows Desktop Composition (only works on Windows Vista and Windows 7 systems).\r\nMay reduce lag and improve performance on some setups.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_DISABLE_DWM).GetHwnd());
      AddToolTip("Activate this if you have issues using an Intel graphics chip.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_SOFTWARE_VP).GetHwnd());
      AddToolTip("1-activates VSYNC for every frame (avoids tearing)\r\n2-adaptive VSYNC, waits only for fast frames (e.g. over 60fps)\r\nor set it to e.g. 60 or 120 to limit the fps to that value (energy saving/less heat)", hwndDlg, toolTipHwnd, GetDlgItem(IDC_ADAPTIVE_VSYNC).GetHwnd());
      AddToolTip("Leave at 0 if you have enabled 'Low Latency' or 'Anti Lag' settings in the graphics driver.\r\nOtherwise experiment with 1 or 2 for a chance of lag reduction at the price of a bit of framerate.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_MAX_PRE_FRAMES).GetHwnd());
      AddToolTip("If played in cabinet mode and you get an egg shaped ball activate this.\r\nFor screen ratios other than 16:9 you may have to adjust the offsets.\r\nNormally you have to set the Y offset (around 1.5) but you have to experiment.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_StretchMonitor).GetHwnd());
      AddToolTip("Changes the visual effect/screen shaking when nudging the table.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_NUDGE_STRENGTH).GetHwnd());
      AddToolTip("Activate this to switch the table brightness automatically based on your PC date,clock and location.\r\nThis requires to fill in geographic coordinates for your PCs location to work correctly.\r\nYou may use openstreetmap.org for example to get these in the correct format.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_DYNAMIC_DN).GetHwnd());
      AddToolTip("In decimal degrees (-90..90, North positive)", hwndDlg, toolTipHwnd, GetDlgItem(IDC_DN_LATITUDE).GetHwnd());
      AddToolTip("In decimal degrees (-180..180, East positive)", hwndDlg, toolTipHwnd, GetDlgItem(IDC_DN_LONGITUDE).GetHwnd());
      AddToolTip("Activate this to enable dynamic Ambient Occlusion.\r\nThis slows down performance, but enables contact shadows for dynamic objects.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_DYNAMIC_AO).GetHwnd());
      AddToolTip("Activate this to enhance the texture filtering.\r\nThis slows down performance only a bit (on most systems), but increases quality tremendously.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_FORCE_ANISO).GetHwnd());
      AddToolTip("Activate this to enable Ambient Occlusion.\r\nThis enables contact shadows between objects.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_ENABLE_AO).GetHwnd());
      AddToolTip("Activate this to enable 3D Stereo output using the requested format.\r\nSwitch on/off during play with the F10 key.\r\nThis requires that your TV can display 3D Stereo, and respective 3D glasses.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_3D_STEREO).GetHwnd());
      AddToolTip("Switches 3D Stereo effect to use the Y Axis.\r\nThis should usually be selected for Cabinets/rotated displays.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_3D_STEREO_Y).GetHwnd());
      if (IsWindows10_1803orAbove())
      {
         GetDlgItem(IDC_FULLSCREEN).SetWindowText("Force exclusive Fullscreen Mode");
         AddToolTip("Enforces exclusive Fullscreen Mode.\r\nEnforcing exclusive FS can slightly reduce input lag.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_FULLSCREEN).GetHwnd());
      }
      else
         AddToolTip("Enforces exclusive Fullscreen Mode.\r\nDo not enable if you require to see the VPinMAME or B2S windows for example.\r\nEnforcing exclusive FS can slightly reduce input lag though.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_FULLSCREEN).GetHwnd());
      AddToolTip("Enforces 10Bit (WCG) rendering.\r\nRequires a corresponding 10Bit output capable graphics card and monitor.\r\nAlso requires to have exclusive fullscreen mode enforced (for now).", hwndDlg, toolTipHwnd, GetDlgItem(IDC_10BIT_VIDEO).GetHwnd());
      AddToolTip("Switches all tables to use the respective Cabinet display setup.\r\nAlso useful if a 270 degree rotated Desktop monitor is used.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_BG_SET).GetHwnd());
      AddToolTip("Enables post-processed Anti-Aliasing.\r\nThis delivers smoother images, at the cost of slight blurring.\r\n'Quality FXAA' and 'Quality SMAA' are recommended and lead to less artifacts,\nbut will harm performance on low-end graphics cards.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_FXAACB).GetHwnd());
      AddToolTip("Enables post-processed Sharpening of the image.\r\n'Bilateral CAS' is recommended,\nbut will harm performance on low-end graphics cards.\r\n'CAS' is less aggressive and faster, but also rather subtle.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_SHARPENCB).GetHwnd());
      AddToolTip("Enables brute-force 4x Anti-Aliasing (similar to DSR).\r\nThis delivers very good quality, but slows down performance significantly.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_AA_ALL_TABLES).GetHwnd());
      AddToolTip("When checked it overwrites the ball image/decal image(s) for every table.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_OVERWRITE_BALL_IMAGE_CHECK).GetHwnd());
      AddToolTip("Select Display for Video output.", hwndDlg, toolTipHwnd, GetDlgItem(IDC_DISPLAY_ID).GetHwnd());
   }

   const int maxTexDim = LoadValueIntWithDefault("Player", "MaxTexDimension", 0); // default: Don't resize textures
   switch (maxTexDim)
   {
      case 3072:SendMessage(GetDlgItem(IDC_Tex3072).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0);      break;
      case 512: // legacy, map to 1024 nowadays
      case 1024:SendMessage(GetDlgItem(IDC_Tex1024).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0);      break;
      case 2048:SendMessage(GetDlgItem(IDC_Tex2048).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0);      break;
      default:	SendMessage(GetDlgItem(IDC_TexUnlimited).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0); break;
   }

   const bool reflection = LoadValueBoolWithDefault("Player", "BallReflection", true);
   SendMessage(GetDlgItem(IDC_GLOBAL_REFLECTION_CHECK).GetHwnd(), BM_SETCHECK, reflection ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool trail = LoadValueBoolWithDefault("Player", "BallTrail", true);
   SendMessage(GetDlgItem(IDC_GLOBAL_TRAIL_CHECK).GetHwnd(), BM_SETCHECK, trail ? BST_CHECKED : BST_UNCHECKED, 0);

   const int vsync = LoadValueIntWithDefault("Player", "AdaptiveVSync", 0);
   SetDlgItemInt(IDC_ADAPTIVE_VSYNC, vsync, FALSE);

   const int maxPrerenderedFrames = LoadValueIntWithDefault("Player", "MaxPrerenderedFrames", 0);
   SetDlgItemInt(IDC_MAX_PRE_FRAMES, maxPrerenderedFrames, FALSE);

   char tmp[256];

   const float ballAspecRatioOffsetX = LoadValueFloatWithDefault("Player", "BallCorrectionX", 0.f);
   sprintf_s(tmp, 256, "%f", ballAspecRatioOffsetX);
   SetDlgItemTextA(IDC_CORRECTION_X, tmp);

   const float ballAspecRatioOffsetY = LoadValueFloatWithDefault("Player", "BallCorrectionY", 0.f);
   sprintf_s(tmp, 256, "%f", ballAspecRatioOffsetY);
   SetDlgItemTextA(IDC_CORRECTION_Y, tmp);

   const float latitude = LoadValueFloatWithDefault("Player", "Latitude", 52.52f);
   sprintf_s(tmp, 256, "%f", latitude);
   SetDlgItemTextA(IDC_DN_LATITUDE, tmp);

   const float longitude = LoadValueFloatWithDefault("Player", "Longitude", 13.37f);
   sprintf_s(tmp, 256, "%f", longitude);
   SetDlgItemTextA(IDC_DN_LONGITUDE, tmp);

   const float nudgeStrength = LoadValueFloatWithDefault("Player", "NudgeStrength", 2e-2f);
   sprintf_s(tmp, 256, "%f", nudgeStrength);
   SetDlgItemTextA(IDC_NUDGE_STRENGTH, tmp);

   const int useAA = LoadValueIntWithDefault("Player", "USEAA", 0);
   SendMessage(GetDlgItem(IDC_AA_ALL_TABLES).GetHwnd(), BM_SETCHECK, (useAA != 0) ? BST_CHECKED : BST_UNCHECKED, 0);

   const int useDN = LoadValueIntWithDefault("Player", "DynamicDayNight", 0);
   SendMessage(GetDlgItem(IDC_DYNAMIC_DN).GetHwnd(), BM_SETCHECK, (useDN != 0) ? BST_CHECKED : BST_UNCHECKED, 0);

   bool useAO = LoadValueBoolWithDefault("Player", "DynamicAO", false);
   SendMessage(GetDlgItem(IDC_DYNAMIC_AO).GetHwnd(), BM_SETCHECK, useAO ? BST_CHECKED : BST_UNCHECKED, 0);

   useAO = LoadValueBoolWithDefault("Player", "DisableAO", false);
   SendMessage(GetDlgItem(IDC_ENABLE_AO).GetHwnd(), BM_SETCHECK, useAO ? BST_UNCHECKED : BST_CHECKED, 0); // inverted logic

   const bool ssreflection = LoadValueBoolWithDefault("Player", "SSRefl", false);
   SendMessage(GetDlgItem(IDC_GLOBAL_SSREFLECTION_CHECK).GetHwnd(), BM_SETCHECK, ssreflection ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool pfreflection = LoadValueBoolWithDefault("Player", "PFRefl", true);
   SendMessage(GetDlgItem(IDC_GLOBAL_PFREFLECTION_CHECK).GetHwnd(), BM_SETCHECK, pfreflection ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool overwiteBallImage = LoadValueBoolWithDefault("Player", "OverwriteBallImage", false);
   SendMessage(GetDlgItem(IDC_OVERWRITE_BALL_IMAGE_CHECK).GetHwnd(), BM_SETCHECK, overwiteBallImage ? BST_CHECKED : BST_UNCHECKED, 0);

   string imageName;
   HRESULT hr = LoadValue("Player", "BallImage", imageName);
   if (hr != S_OK)
      imageName.clear();
   SetDlgItemText(IDC_BALL_IMAGE_EDIT, imageName.c_str());
   hr = LoadValue("Player", "DecalImage", imageName);
   if (hr != S_OK)
      imageName.clear();
   SetDlgItemText(IDC_BALL_DECAL_EDIT, imageName.c_str());
   if (overwiteBallImage == 0)
   {
      ::EnableWindow(GetDlgItem(IDC_BROWSE_BALL_IMAGE).GetHwnd(), FALSE);
      ::EnableWindow(GetDlgItem(IDC_BROWSE_BALL_DECAL).GetHwnd(), FALSE);
      ::EnableWindow(GetDlgItem(IDC_BALL_IMAGE_EDIT).GetHwnd(), FALSE);
      ::EnableWindow(GetDlgItem(IDC_BALL_DECAL_EDIT).GetHwnd(), FALSE);
   }

   const int fxaa = LoadValueIntWithDefault("Player", "FXAA", Standard_FXAA);
   HWND hwnd = GetDlgItem(IDC_FXAACB).GetHwnd();
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Disabled");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Fast FXAA");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Standard FXAA");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Quality FXAA");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Fast NFAA");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Standard DLAA");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Quality SMAA");
   SendMessage(hwnd, CB_SETCURSEL, fxaa, 0);

   const int sharpen = LoadValueIntWithDefault("Player", "Sharpen", 0);
   hwnd = GetDlgItem(IDC_SHARPENCB).GetHwnd();
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Disabled");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"CAS");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Bilateral CAS");
   SendMessage(hwnd, CB_SETCURSEL, sharpen, 0);

   const bool scaleFX_DMD = LoadValueBoolWithDefault("Player", "ScaleFXDMD", false);
   SendMessage(GetDlgItem(IDC_SCALE_FX_DMD).GetHwnd(), BM_SETCHECK, scaleFX_DMD ? BST_CHECKED : BST_UNCHECKED, 0);

   const int bgset = LoadValueIntWithDefault("Player", "BGSet", 0);
   SendMessage(GetDlgItem(IDC_BG_SET).GetHwnd(), BM_SETCHECK, (bgset != 0) ? BST_CHECKED : BST_UNCHECKED, 0);

   const int stereo3D = LoadValueIntWithDefault("Player", "Stereo3D", 0);
   hwnd = GetDlgItem(IDC_3D_STEREO).GetHwnd();
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Disabled");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"TB (Top / Bottom)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Interlaced (e.g. LG TVs)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Flipped Interlaced (e.g. LG TVs)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"SBS (Side by Side)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Red/Cyan");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Green/Magenta");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Dubois Red/Cyan");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Dubois Green/Magenta");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Deghosted Red/Cyan");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Deghosted Green/Magenta");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Blue/Amber");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Cyan/Red");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Magenta/Green");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Dubois Cyan/Red");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Dubois Magenta/Green");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Deghosted Cyan/Red");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Deghosted Magenta/Green");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"Anaglyph Amber/Blue");
   SendMessage(hwnd, CB_SETCURSEL, stereo3D, 0);

   const bool stereo3DY = LoadValueBoolWithDefault("Player", "Stereo3DYAxis", false);
   SendMessage(GetDlgItem(IDC_3D_STEREO_Y).GetHwnd(), BM_SETCHECK, stereo3DY ? BST_CHECKED : BST_UNCHECKED, 0);

   const float stereo3DOfs = LoadValueFloatWithDefault("Player", "Stereo3DOffset", 0.f);
   sprintf_s(tmp, 256, "%f", stereo3DOfs);
   SetDlgItemTextA(IDC_3D_STEREO_OFS, tmp);

   const float stereo3DMS = LoadValueFloatWithDefault("Player", "Stereo3DMaxSeparation", 0.03f);
   sprintf_s(tmp, 256, "%f", stereo3DMS);
   SetDlgItemTextA(IDC_3D_STEREO_MS, tmp);

   const float stereo3DZPD = LoadValueFloatWithDefault("Player", "Stereo3DZPD", 0.5f);
   sprintf_s(tmp, 256, "%f", stereo3DZPD);
   SetDlgItemTextA(IDC_3D_STEREO_ZPD, tmp);

   const float stereo3DContrast = LoadValueFloatWithDefault("Player", "Stereo3DContrast", 1.0f);
   sprintf_s(tmp, 256, "%f", stereo3DContrast);
   SetDlgItemTextA(IDC_3D_STEREO_CONTRAST, tmp);

   const float stereo3DDesaturation = LoadValueFloatWithDefault("Player", "Stereo3DDesaturation", 0.0f);
   sprintf_s(tmp, 256, "%f", stereo3DDesaturation);
   SetDlgItemTextA(IDC_3D_STEREO_DESATURATION, tmp);

   const bool disableDWM = LoadValueBoolWithDefault("Player", "DisableDWM", false);
   SendMessage(GetDlgItem(IDC_DISABLE_DWM).GetHwnd(), BM_SETCHECK, disableDWM ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool nvidiaApi = LoadValueBoolWithDefault("Player", "UseNVidiaAPI", false);
   SendMessage(GetDlgItem(IDC_USE_NVIDIA_API_CHECK).GetHwnd(), BM_SETCHECK, nvidiaApi ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool bloomOff = LoadValueBoolWithDefault("Player", "ForceBloomOff", false);
   SendMessage(GetDlgItem(IDC_BLOOM_OFF).GetHwnd(), BM_SETCHECK, bloomOff ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool forceAniso = LoadValueBoolWithDefault("Player", "ForceAnisotropicFiltering", true);
   SendMessage(GetDlgItem(IDC_FORCE_ANISO).GetHwnd(), BM_SETCHECK, forceAniso ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool compressTextures = LoadValueBoolWithDefault("Player", "CompressTextures", false);
   SendMessage(GetDlgItem(IDC_TEX_COMPRESS).GetHwnd(), BM_SETCHECK, compressTextures ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool softwareVP = LoadValueBoolWithDefault("Player", "SoftwareVertexProcessing", false);
   SendMessage(GetDlgItem(IDC_SOFTWARE_VP).GetHwnd(), BM_SETCHECK, softwareVP ? BST_CHECKED : BST_UNCHECKED, 0);

   const bool video10bit = LoadValueBoolWithDefault("Player", "Render10Bit", false);
   SendMessage(GetDlgItem(IDC_10BIT_VIDEO).GetHwnd(), BM_SETCHECK, video10bit ? BST_CHECKED : BST_UNCHECKED, 0);

   const int depthcur = LoadValueIntWithDefault("Player", "ColorDepth", 32);
   const int refreshrate = LoadValueIntWithDefault("Player", "RefreshRate", 0);

   int display;
   hr = LoadValue("Player", "Display", display);
   std::vector<DisplayConfig> displays;
   getDisplayList(displays);
   if ((hr != S_OK) || ((int)displays.size() <= display))
      display = -1;

   hwnd = GetDlgItem(IDC_DISPLAY_ID).GetHwnd();
   SendMessage(hwnd, CB_RESETCONTENT, 0, 0);

   for (std::vector<DisplayConfig>::iterator dispConf = displays.begin(); dispConf != displays.end(); ++dispConf)
   {
      if (display == -1 && dispConf->isPrimary)
         display = dispConf->display;
      char displayName[256];
      sprintf_s(displayName, "Display %d%s %dx%d %s", dispConf->display + 1, (dispConf->isPrimary) ? "*" : "", dispConf->width, dispConf->height, dispConf->GPU_Name);
      SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)displayName);
   }
   SendMessage(hwnd, CB_SETCURSEL, display, 0);

   const bool fullscreen = LoadValueBoolWithDefault("Player", "FullScreen", IsWindows10_1803orAbove());
   SendMessage(hwndDlg, fullscreen ? GET_FULLSCREENMODES : GET_WINDOW_MODES, 0, 0);
   SendMessage(GetDlgItem(IDC_FULLSCREEN).GetHwnd(), BM_SETCHECK, fullscreen ? BST_CHECKED : BST_UNCHECKED, 0);

   const int alphaRampsAccuracy = LoadValueIntWithDefault("Player", "AlphaRampAccuracy", 10);
   const HWND hwndARASlider = GetDlgItem(IDC_ARASlider).GetHwnd();
   SendMessage(hwndARASlider, TBM_SETRANGE, fTrue, MAKELONG(0, 10));
   SendMessage(hwndARASlider, TBM_SETTICFREQ, 1, 0);
   SendMessage(hwndARASlider, TBM_SETLINESIZE, 0, 1);
   SendMessage(hwndARASlider, TBM_SETPAGESIZE, 0, 1);
   SendMessage(hwndARASlider, TBM_SETTHUMBLENGTH, 5, 0);
   SendMessage(hwndARASlider, TBM_SETPOS, TRUE, alphaRampsAccuracy);

   const int ballStretchMode = LoadValueIntWithDefault("Player", "BallStretchMode", 0);
   switch (ballStretchMode)
   {
      default:
      case 0:  SendMessage(GetDlgItem(IDC_StretchNo).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0);      break;
      case 1:  SendMessage(GetDlgItem(IDC_StretchYes).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0);     break;
      case 2:  SendMessage(GetDlgItem(IDC_StretchMonitor).GetHwnd(), BM_SETCHECK, BST_CHECKED, 0); break;
   }

   // set selected Monitors
   // Monitors: 4:3, 16:9, 16:10, 21:10, 21:9
   /*const int selected = LoadValueIntWithDefault("Player", "BallStretchMonitor", 1); // assume 16:9 as standard
   HWND hwnd = GetDlgItem(IDC_MonitorCombo).GetHwnd();
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"4:3");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"16:9");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"16:10");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"21:10");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"3:4 (R)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"9:16 (R)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"10:16 (R)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"10:21 (R)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"9:21 (R)");
   SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"21:9");
   SendMessage(hwnd, CB_SETCURSEL, selected, 0);*/

   return TRUE;
}

INT_PTR VideoOptionsDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
      case GET_WINDOW_MODES:
      {
         size_t indx = -1;

         const HWND hwndList = GetDlgItem(IDC_SIZELIST).GetHwnd();
         //SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
         //indx = SendMessage(hwndList, LB_GETCURSEL, 0L, 0L);
         //if (indx == LB_ERR)
         //  indx = 0;

         const int display = (int)SendMessage(GetDlgItem(IDC_DISPLAY_ID).GetHwnd(), CB_GETCURSEL, 0, 0);
         int screenwidth, screenheight;
         int x, y;
         getDisplaySetupByID(display, x, y, screenwidth, screenheight);

         const int widthcur  = LoadValueIntWithDefault("Player", "Width", DEFAULT_PLAYER_WIDTH);
         const int heightcur = LoadValueIntWithDefault("Player", "Height", widthcur * 9 / 16);

         allVideoModes.clear();

         // test video modes first on list

         // add some (windowed) portrait play modes

         // 16:10 aspect ratio resolutions: 1280*800, 1440*900, 1680*1050, 1920*1200 and 2560*1600
         // 16:9 aspect ratio resolutions:  1280*720, 1366*768, 1600*900, 1920*1080, 2560*1440 and 3840*2160
         // 21:9 aspect ratio resolutions:  3440*1440,2560*1080
         // 21:10 aspect ratio resolution:  3840*1600
         // 4:3  aspect ratio resolutions:  1280*1024
         constexpr unsigned int num_portrait_modes = 33;
         static constexpr int portrait_modes_width[num_portrait_modes] =  {431, 505, 720, 768, 606, 864, 600, 720, 768, 960,1024, 768, 768, 800,1050, 808, 900, 900,1050,1200,1050,1080,1200,1536,1212,1080,1440,1440,1600,1920,2048,1600,2160};
         static constexpr int portrait_modes_height[num_portrait_modes] = {768, 900,1024,1024,1080,1152,1280,1280,1280,1280,1280,1360,1366,1280,1400,1440,1440,1600,1600,1600,1680,1920,1920,2048,2160,2560,2560,3440,2560,2560,2560,3840,3840};

         for (unsigned int i = 0; i < num_portrait_modes; ++i)
            if ((portrait_modes_width[i] <= screenwidth) && (portrait_modes_height[i] <= screenheight)
                && ((portrait_modes_width[i] != screenwidth) || (portrait_modes_height[i] != screenheight))) // windowed fullscreen is added to the end separately
            {
               VideoMode mode;
               mode.width = portrait_modes_width[i];
               mode.height = portrait_modes_height[i];
               mode.depth = 0;
               mode.refreshrate = 0;

               if (heightcur > widthcur)
                  if ((portrait_modes_width[i] == widthcur) && (portrait_modes_height[i] == heightcur))
                     indx = allVideoModes.size();
               allVideoModes.push_back(mode);
            }

         // add landscape play modes
         static constexpr int rgwindowsize[] = { 640, 720, 800, 912, 1024, 1152, 1280, 1360, 1366, 1400, 1440, 1600, 1680, 1920, 2048, 2560, 3440, 3840, 4096, 5120, 6400, 7680, 8192, 11520, 15360 };

         for (size_t i = 0; i < sizeof(rgwindowsize)/sizeof(int) * 5; ++i)
         {
            const int xsize = rgwindowsize[i/5];
            
            int mulx = 1, divy = 1;
            switch (i%5)
            {
               case 0: mulx = 3;  divy = 4;  break;
               case 1: mulx = 9;  divy = 16; break;
               case 2: mulx = 10; divy = 16; break;
               case 3: mulx = 9;  divy = 21; break;
               case 4: mulx = 10; divy = 21; break;
            }

            const int ysize = xsize * mulx / divy;

            if ((xsize <= screenwidth) && (ysize <= screenheight)
                && ((xsize != screenwidth) || (ysize != screenheight))) // windowed fullscreen is added to the end separately
            {
               if ((xsize == widthcur) && (ysize == heightcur))
                  indx = allVideoModes.size();

               VideoMode mode;
               mode.width = xsize;
               mode.height = ysize;
               mode.depth = 0;
               mode.refreshrate = 0;

               allVideoModes.push_back(mode);
            }
         }

         FillVideoModesList(allVideoModes);

         // add windowed fullscreen mode at the end
         VideoMode mode;
         mode.width = screenwidth;
         mode.height = screenheight;
         mode.depth = 0;
         mode.refreshrate = 0;

         char szT[128];
         sprintf_s(szT, "%d x %d (Windowed Fullscreen)", mode.width, mode.height);
         SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)szT);
         if (indx == -1 || (mode.width == widthcur && mode.height == heightcur))
            indx = allVideoModes.size();
         allVideoModes.push_back(mode);

         SendMessage(hwndList, LB_SETCURSEL, (indx != -1) ? indx : 0, 0);
         break;
      } // end case GET_WINDOW_MODES
      case GET_FULLSCREENMODES:
      {
         const HWND hwndList = GetDlgItem(IDC_SIZELIST).GetHwnd();
         const int display = (int)SendMessage(GetDlgItem(IDC_DISPLAY_ID).GetHwnd(), CB_GETCURSEL, 0, 0);
         EnumerateDisplayModes(display, allVideoModes);

         const int depthcur = LoadValueIntWithDefault("Player", "ColorDepth", 32);
         const int refreshrate = LoadValueIntWithDefault("Player", "RefreshRate", 0);
         const int widthcur = LoadValueIntWithDefault("Player", "Width", -1);
         const int heightcur = LoadValueIntWithDefault("Player", "Height", -1);

         VideoMode curSelMode;
         curSelMode.width = widthcur;
         curSelMode.height = heightcur;
         curSelMode.depth = depthcur;
         curSelMode.refreshrate = refreshrate;

         FillVideoModesList(allVideoModes, &curSelMode);

         if (SendMessage(hwndList, LB_GETCURSEL, 0, 0) == -1)
            SendMessage(hwndList, LB_SETCURSEL, 0, 0);
         break;
      }
   }

   return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL VideoOptionsDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
   UNREFERENCED_PARAMETER(lParam);

   switch (LOWORD(wParam))
   {
      case IDC_DEFAULTS:
      {
         ResetVideoPreferences(0);
         break;
      }
      case IDC_DEFAULTS_LOW:
      {
         ResetVideoPreferences(1);
         break;
      }
      case IDC_DEFAULTS_HIGH:
      {
         ResetVideoPreferences(2);
         break;
      }
      case IDC_RESET_WINDOW:
      {
         (void)DeleteValue("Player", "WindowPosX");
         (void)DeleteValue("Player", "WindowPosY");
         break;
      }
      case IDC_OVERWRITE_BALL_IMAGE_CHECK:
      {
         const BOOL overwriteEnabled = (IsDlgButtonChecked(IDC_OVERWRITE_BALL_IMAGE_CHECK) == BST_CHECKED) ? TRUE : FALSE;
         ::EnableWindow(GetDlgItem(IDC_BROWSE_BALL_IMAGE).GetHwnd(), overwriteEnabled);
         ::EnableWindow(GetDlgItem(IDC_BROWSE_BALL_DECAL).GetHwnd(), overwriteEnabled);
         ::EnableWindow(GetDlgItem(IDC_BALL_IMAGE_EDIT).GetHwnd(), overwriteEnabled);
         ::EnableWindow(GetDlgItem(IDC_BALL_DECAL_EDIT).GetHwnd(), overwriteEnabled);
         break;
      }
      case IDC_BROWSE_BALL_IMAGE:
      {
         char szFileName[MAXSTRING];
         szFileName[0] = '\0';

         OPENFILENAME ofn = {};
         ofn.lStructSize = sizeof(OPENFILENAME);
         ofn.hInstance = g_pvp->theInstance;
         ofn.hwndOwner = g_pvp->GetHwnd();
         // TEXT
         ofn.lpstrFilter = "Bitmap, JPEG, PNG, TGA, WEBP, EXR, HDR Files (.bmp/.jpg/.png/.tga/.webp/.exr/.hdr)\0*.bmp;*.jpg;*.jpeg;*.png;*.tga;*.webp;*.exr;*.hdr\0";
         ofn.lpstrFile = szFileName;
         ofn.nMaxFile = sizeof(szFileName);
         ofn.lpstrDefExt = "png";
         ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
         const int ret = GetOpenFileName(&ofn);
         if (!ret)
            break;
         SetDlgItemText(IDC_BALL_IMAGE_EDIT, szFileName);

         break;
      }
      case IDC_BROWSE_BALL_DECAL:
      {
         char szFileName[MAXSTRING];
         szFileName[0] = '\0';

         OPENFILENAME ofn = {};
         ofn.lStructSize = sizeof(OPENFILENAME);
         ofn.hInstance = g_pvp->theInstance;
         ofn.hwndOwner = g_pvp->GetHwnd();
         // TEXT
         ofn.lpstrFilter = "Bitmap, JPEG, PNG, TGA, WEBP, EXR, HDR Files (.bmp/.jpg/.png/.tga/.webp/.exr/.hdr)\0*.bmp;*.jpg;*.jpeg;*.png;*.tga;*.webp;*.exr;*.hdr\0";
         ofn.lpstrFile = szFileName;
         ofn.nMaxFile = sizeof(szFileName);
         ofn.lpstrDefExt = "png";
         ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
         const int ret = GetOpenFileName(&ofn);
         if (!ret)
            break;
         SetDlgItemText(IDC_BALL_DECAL_EDIT, szFileName);

         break;
      }
      case IDC_DISPLAY_ID:
      case IDC_FULLSCREEN:
      {
         const size_t checked = SendDlgItemMessage(IDC_FULLSCREEN, BM_GETCHECK, 0, 0);
         SendMessage(checked ? GET_FULLSCREENMODES : GET_WINDOW_MODES, 0, 0);
         break;
      }
      default:
         return FALSE;
   }
   return TRUE;
}

void VideoOptionsDialog::OnOK()
{
   BOOL nothing = 0;

   const bool fullscreen = (SendMessage(GetDlgItem(IDC_FULLSCREEN).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "FullScreen", fullscreen);

   const size_t index = SendMessage(GetDlgItem(IDC_SIZELIST).GetHwnd(), LB_GETCURSEL, 0, 0);
   const VideoMode* const pvm = &allVideoModes[index];
   SaveValueInt("Player", "Width", pvm->width);
   SaveValueInt("Player", "Height", pvm->height);
   if (fullscreen)
   {
      SaveValueInt("Player", "ColorDepth", pvm->depth);
      SaveValueInt("Player", "RefreshRate", pvm->refreshrate);
   }
   const size_t display = SendMessage(GetDlgItem(IDC_DISPLAY_ID).GetHwnd(), CB_GETCURSEL, 0, 0);
   SaveValueInt("Player", "Display", (int)display);

   const bool video10bit = (SendMessage(GetDlgItem(IDC_10BIT_VIDEO).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "Render10Bit", video10bit);

   //const HWND maxTexDimUnlimited = GetDlgItem(hwndDlg, IDC_TexUnlimited);
   int maxTexDim = 0;
   if (SendMessage(GetDlgItem(IDC_Tex3072).GetHwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED)
      maxTexDim = 3072;
   if (SendMessage(GetDlgItem(IDC_Tex1024).GetHwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED)
      maxTexDim = 1024;
   if (SendMessage(GetDlgItem(IDC_Tex2048).GetHwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED)
      maxTexDim = 2048;
   SaveValueInt("Player", "MaxTexDimension", maxTexDim);

   const bool reflection = (SendMessage(GetDlgItem(IDC_GLOBAL_REFLECTION_CHECK).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "BallReflection", reflection);

   const bool trail = (SendMessage(GetDlgItem(IDC_GLOBAL_TRAIL_CHECK).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "BallTrail", trail);

   const int vsync = GetDlgItemInt(IDC_ADAPTIVE_VSYNC, nothing, TRUE);
   SaveValueInt("Player", "AdaptiveVSync", vsync);

   const int maxPrerenderedFrames = GetDlgItemInt(IDC_MAX_PRE_FRAMES, nothing, TRUE);
   SaveValueInt("Player", "MaxPrerenderedFrames", maxPrerenderedFrames);

   CString tmpStr;
   tmpStr = GetDlgItemTextA(IDC_CORRECTION_X);
   SaveValue("Player", "BallCorrectionX", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_CORRECTION_Y);
   SaveValue("Player", "BallCorrectionY", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_DN_LONGITUDE);
   SaveValue("Player", "Longitude", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_DN_LATITUDE);
   SaveValue("Player", "Latitude", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_NUDGE_STRENGTH);
   SaveValue("Player", "NudgeStrength", tmpStr);

   size_t fxaa = SendMessage(GetDlgItem(IDC_FXAACB).GetHwnd(), CB_GETCURSEL, 0, 0);
   if (fxaa == LB_ERR)
      fxaa = Standard_FXAA;
   SaveValueInt("Player", "FXAA", (int)fxaa);

   size_t sharpen = SendMessage(GetDlgItem(IDC_SHARPENCB).GetHwnd(), CB_GETCURSEL, 0, 0);
   if (sharpen == LB_ERR)
      sharpen = 0;
   SaveValueInt("Player", "Sharpen", (int)sharpen);

   const bool scaleFX_DMD = (SendMessage(GetDlgItem(IDC_SCALE_FX_DMD).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "ScaleFXDMD", scaleFX_DMD);

   const size_t BGSet = SendMessage(GetDlgItem(IDC_BG_SET).GetHwnd(), BM_GETCHECK, 0, 0);
   SaveValueInt("Player", "BGSet", (int)BGSet);

   const size_t useAA = SendMessage(GetDlgItem(IDC_AA_ALL_TABLES).GetHwnd(), BM_GETCHECK, 0, 0);
   SaveValueInt("Player", "USEAA", (int)useAA);

   const bool useDN = (SendMessage(GetDlgItem(IDC_DYNAMIC_DN).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "DynamicDayNight", useDN);

   bool useAO = (SendMessage(GetDlgItem(IDC_DYNAMIC_AO).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "DynamicAO", useAO);

   useAO = SendMessage(GetDlgItem(IDC_ENABLE_AO).GetHwnd(), BM_GETCHECK, 0, 0) ? false : true; // inverted logic
   SaveValueBool("Player", "DisableAO", useAO);

   const bool ssreflection = (SendMessage(GetDlgItem(IDC_GLOBAL_SSREFLECTION_CHECK).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "SSRefl", ssreflection);

   const bool pfreflection = (SendMessage(GetDlgItem(IDC_GLOBAL_PFREFLECTION_CHECK).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "PFRefl", pfreflection);

   size_t stereo3D = SendMessage(GetDlgItem(IDC_3D_STEREO).GetHwnd(), CB_GETCURSEL, 0, 0);
   if (stereo3D == LB_ERR)
      stereo3D = 0;
   SaveValueInt("Player", "Stereo3D", (int)stereo3D);
   SaveValueInt("Player", "Stereo3DEnabled", (int)stereo3D);

   const bool stereo3DY = (SendMessage(GetDlgItem(IDC_3D_STEREO_Y).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "Stereo3DYAxis", stereo3DY);

   const bool forceAniso = (SendMessage(GetDlgItem(IDC_FORCE_ANISO).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "ForceAnisotropicFiltering", forceAniso);

   const bool texCompress = (SendMessage(GetDlgItem(IDC_TEX_COMPRESS).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "CompressTextures", texCompress);

   const bool softwareVP = (SendMessage(GetDlgItem(IDC_SOFTWARE_VP).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "SoftwareVertexProcessing", softwareVP);

   const size_t alphaRampsAccuracy = SendMessage(GetDlgItem(IDC_ARASlider).GetHwnd(), TBM_GETPOS, 0, 0);
   SaveValueInt("Player", "AlphaRampAccuracy", (int)alphaRampsAccuracy);

   tmpStr = GetDlgItemTextA(IDC_3D_STEREO_OFS);
   SaveValue("Player", "Stereo3DOffset", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_3D_STEREO_MS);
   SaveValue("Player", "Stereo3DMaxSeparation", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_3D_STEREO_ZPD);
   SaveValue("Player", "Stereo3DZPD", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_3D_STEREO_CONTRAST);
   SaveValue("Player", "Stereo3DContrast", tmpStr);

   tmpStr = GetDlgItemTextA(IDC_3D_STEREO_DESATURATION);
   SaveValue("Player", "Stereo3DDesaturation", tmpStr);

   const bool disableDWM = (SendMessage(GetDlgItem(IDC_DISABLE_DWM).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "DisableDWM", disableDWM);

   const bool nvidiaApi = (SendMessage(GetDlgItem(IDC_USE_NVIDIA_API_CHECK).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "UseNVidiaAPI", nvidiaApi);

   const bool bloomOff = (SendMessage(GetDlgItem(IDC_BLOOM_OFF).GetHwnd(), BM_GETCHECK, 0, 0) != 0);
   SaveValueBool("Player", "ForceBloomOff", bloomOff);

   //HWND hwndBallStretchNo = GetDlgItem(hwndDlg, IDC_StretchNo);
   int ballStretchMode = 0;
   if (SendMessage(GetDlgItem(IDC_StretchYes).GetHwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED)
      ballStretchMode = 1;
   if (SendMessage(GetDlgItem(IDC_StretchMonitor).GetHwnd(), BM_GETCHECK, 0, 0) == BST_CHECKED)
      ballStretchMode = 2;
   SaveValueInt("Player", "BallStretchMode", ballStretchMode);

   // get selected Monitors
   // Monitors: 4:3, 16:9, 16:10, 21:10, 21:9
   /*size_t selected = SendMessage(GetDlgItem(IDC_MonitorCombo).GetHwnd(), CB_GETCURSEL, 0, 0);
   if (selected == LB_ERR)
      selected = 1; // assume a 16:9 Monitor as standard
   SaveValueInt("Player", "BallStretchMonitor", (int)selected);*/

   const bool overwriteEnabled = IsDlgButtonChecked(IDC_OVERWRITE_BALL_IMAGE_CHECK) == BST_CHECKED;
   if (overwriteEnabled)
   {
      SaveValueBool("Player", "OverwriteBallImage", true);
      tmpStr = GetDlgItemText(IDC_BALL_IMAGE_EDIT);
      SaveValue("Player", "BallImage", tmpStr);
      tmpStr = GetDlgItemText(IDC_BALL_DECAL_EDIT);
      SaveValue("Player", "DecalImage", tmpStr);
   }
   else
      SaveValueBool("Player", "OverwriteBallImage", false);

   CDialog::OnOK();
}

void VideoOptionsDialog::OnClose()
{
   SendMessage(GetDlgItem(IDC_SIZELIST).GetHwnd(), LB_RESETCONTENT, 0, 0);
   CDialog::OnClose();
}

#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

class BufferedBitmap : public wxScrolled<wxWindow>
{
public:
    BufferedBitmap(wxWindow *parent, wxWindowID id, const wxBitmap &b, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0)
        : wxScrolled<wxWindow>(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE | wxVSCROLL | wxHSCROLL | style)
    {
        this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows

        this->Bind(wxEVT_PAINT, &BufferedBitmap::OnPaint, this);
        this->SetBitmap(b);
    }

    void OnPaint(wxPaintEvent &evt)
    {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        DoPrepareDC(dc);

        wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

        if (gc)
        {
            // scaling consistent with wxStaticBitmap
            const wxSize drawSize = ToDIP(GetVirtualSize());

            const wxSize bmpSize = GetScaledBitmapSize();

            double w = bmpSize.GetWidth();
            double h = bmpSize.GetHeight();

            double x = (drawSize.GetWidth() - w) / 2;
            double y = (drawSize.GetHeight() - h) / 2;

            gc->DrawBitmap(bitmap, gc->FromDIP(x), gc->FromDIP(y), gc->FromDIP(w), gc->FromDIP(h));

            delete gc;
        }
    }

    void SetBitmap(const wxBitmap &bitmap)
    {
        this->bitmap = bitmap;

        SetScrollRate(FromDIP(10), FromDIP(10));
        SetVirtualSize(FromDIP(GetScaledBitmapSize()));

        this->Refresh();
    }

    const wxBitmap &GetBitmap() const
    {
        return bitmap;
    }

    double GetZoomMultiplier() const
    {
        return pow(ZOOM_FACTOR, zoomLevel);
    }

    double GetZoomPercentage() const
    {
        return GetZoomMultiplier() * 100;
    }

    void ZoomIn()
    {
        auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2, GetClientSize().GetHeight() / 2));

        zoomLevel++;

        CenterAfterZoom(centerPos, centerPos * ZOOM_FACTOR);
        SetVirtualSize(FromDIP(GetScaledBitmapSize()));

        this->Refresh();
    }

    void ZoomOut()
    {
        auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2, GetClientSize().GetHeight() / 2));

        zoomLevel--;

        CenterAfterZoom(centerPos, centerPos * (1.0 / ZOOM_FACTOR));
        SetVirtualSize(FromDIP(GetScaledBitmapSize()));

        this->Refresh();
    }

private:
    wxBitmap bitmap;

    const double ZOOM_FACTOR = 1.1;
    int zoomLevel = 0;

    wxSize GetScaledBitmapSize() const
    {
        const wxSize bmpSize = bitmap.GetSize();
        const double zoom = GetZoomMultiplier();
        return wxSize(bmpSize.GetWidth() * zoom, bmpSize.GetHeight() * zoom);
    }

    void CenterAfterZoom(wxPoint previousCenter, wxPoint currentCenter)
    {
        wxPoint pixelsPerUnit;
        GetScrollPixelsPerUnit(&pixelsPerUnit.x, &pixelsPerUnit.y);

        auto delta = currentCenter - previousCenter;

        auto destX = GetViewStart().x + delta.x / pixelsPerUnit.x;
        auto destY = GetViewStart().y + delta.y / pixelsPerUnit.y;

        Scroll(destX, destY);
    }
};
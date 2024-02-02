#include "../defines.h"
#include <SDL2/SDL.h>
#include "game_defines.h"
#include "../utils/linkedList.h"

void calc_hitbox(SDL_Renderer *renderer, SDL_Texture *txt, SDL_Rect rct, list **ans, int n, long double positive_score, long double negative_score)
{
    SDL_Texture *ret = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rct.w, rct.h);
    SDL_SetRenderTarget(renderer, ret);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(ret, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, txt, &rct, NULL);
    SDL_Color txt_px[rct.w][rct.h];
    SDL_Surface *srf = SDL_CreateRGBSurface(0, rct.w, rct.h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    int err = SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, srf->pixels, srf->pitch);
    if (err < 0)
    {
        fprintf(stderr, "[ERR] error while trying to read texture pixels.\n%s\n", SDL_GetError());
    }
    const Uint8 getPixel_bpp = srf->format->BytesPerPixel;
    for (int i = 0; i < rct.w; i++)
        for (int j = 0; j < rct.h; j++)
        {
            Uint8 *pPixel = (Uint8 *)srf->pixels + j * srf->pitch + i * getPixel_bpp;
            Uint32 pixelData;
            switch (getPixel_bpp)
            {
            case 1:
                pixelData = *pPixel;
                break;
            case 2:
                pixelData = *(Uint16 *)pPixel;
                break;
            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    pixelData = pPixel[0] << 16 | pPixel[1] << 8 | pPixel[2];
                else
                    pixelData = pPixel[0] | pPixel[1] << 8 | pPixel[2] << 16;
                break;
            case 4:
                pixelData = *(Uint32 *)pPixel;
                break;
            default:
                pixelData = 0;
            }
            SDL_GetRGBA(pixelData, srf->format, &txt_px[i][j].r, &txt_px[i][j].g, &txt_px[i][j].b, &txt_px[i][j].a);
        }
    SDL_FreeSurface(srf);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int img_vals[rct.w][rct.h];

    for (int i = 0; i < rct.w; i++)
    {
        for (int j = 0; j < rct.h; j++)
        {
            img_vals[i][j] = (txt_px[i][j].a * txt_px[i][j].a +
                              txt_px[i][j].r * txt_px[i][j].r +
                              txt_px[i][j].g * txt_px[i][j].g +
                              txt_px[i][j].b * txt_px[i][j].b) > 16384
                                 ? positive_score
                                 : negative_score;
        }
    }

    for (int i = 0; i < n; i++)
    {
        int max_score = (int)(-INFINITY);
        SDL_Rect mx_rct;

        int prefix_sum[rct.w + 1][rct.h + 1];
        for (int i = 0; i <= rct.w; i++)
            prefix_sum[i][0] = 0;
        for (int i = 0; i <= rct.h; i++)
            prefix_sum[0][i] = 0;
        for (int i = 0; i < rct.w; i++)
            for (int j = 0; j < rct.h; j++)
                prefix_sum[i + 1][j + 1] = prefix_sum[i][j + 1] + prefix_sum[i + 1][j] - prefix_sum[i][j] + img_vals[i][j];

        for (int l = 0; l < rct.w; l++)
            for (int r = l + 1; r <= rct.w; r++)
                for (int u = 0; u < rct.h; u++)
                    for (int d = u + 1; d <= rct.h; d++)
                    {
                        int score = prefix_sum[r][d] - prefix_sum[r][u] - prefix_sum[l][d] + prefix_sum[l][u];
                        if (score > max_score)
                        {
                            max_score = score;
                            mx_rct.x = l;
                            mx_rct.y = u;
                            mx_rct.w = r - l;
                            mx_rct.h = d - u;
                        }
                    }

        addElement(ans, sizeof(SDL_Rect), 0);
        *((SDL_Rect *)(*ans)->val) = mx_rct;

        for (int i = mx_rct.x; i < mx_rct.x + mx_rct.w; i++)
            for (int j = mx_rct.y; j < mx_rct.y + mx_rct.h; j++)
                img_vals[i][j] = negative_score;
    }
}
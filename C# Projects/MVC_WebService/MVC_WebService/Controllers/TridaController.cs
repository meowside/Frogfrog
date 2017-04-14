using MVC_WebService.Models;
using MVC_WebService.SkolaWS;
using System;
using System.Collections.Generic;
using System.Web.Mvc;

namespace MVC_WebService.Controllers
{
    public class TridaController : Controller
    {
        public ActionResult List()
        {
            SkolaWS.SkolaSoapClient skola = new SkolaWS.SkolaSoapClient();
            List<TridaModel> seznamTrid = new List<TridaModel>();

            foreach (SkolaTridy skolaTridy in skola.TridaList())
            {
                TridaModel trida = new TridaModel
                {
                    Id          = skolaTridy.Id,
                    Nazev       = skolaTridy.Nazev,
                    Tridni      = skolaTridy.Tridni,
                    ZmenenoKdy  = skolaTridy.ZmenenoKdy,
                    ZmenenoKdo  = skolaTridy.ZmenenoKdo,
                    Rocnik      = skolaTridy.Rocnik,
                };

                seznamTrid.Add(trida);
            }

            ViewBag.PocetTrid = seznamTrid.Count;
            return View(seznamTrid);
        }

        [HttpGet]
        public ActionResult Create()
        {
            TridaModel trida = new TridaModel
            {
                ZmenenoKdo  = "( ͡° ͜ʖ ͡°)",
                Ulozeno     = false
            };

            return View(trida);
        }

        [HttpPost]
        public ActionResult Create(TridaModel trida)
        {
            SkolaTridy skolaTridy = new SkolaTridy
            {
                Id          = Guid.NewGuid(),
                Nazev       = trida.Nazev,
                Rocnik      = trida.Rocnik,
                Tridni      = trida.Tridni,
                ZmenenoKdy  = DateTime.Now,
                ZmenenoKdo  = trida.ZmenenoKdo
            };

            new SkolaSoapClient().TridaCreate(skolaTridy);

            trida.Ulozeno = true;
            return View(trida);
        }

        public ActionResult Delete(string id)
        {
            new SkolaSoapClient().TridaSmazat(id);
            return RedirectToAction("List");
        }
    }
}
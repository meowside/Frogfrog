using MVC_WebService.Models;
using MVC_WebService.SkolaWS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web.Mvc;

namespace MVC_WebService.Controllers
{
    public class ZakController : Controller
    {
        public ActionResult List()
        {
            SkolaSoapClient skola = new SkolaSoapClient();
            List<ZakLocalModel> seznamZaku = new List<ZakLocalModel>();

            foreach (ZakModel zakModel in skola.ZaciList())
            {
                seznamZaku.Add(new ZakLocalModel
                {
                    Id          = zakModel.Id,
                    Jmeno       = zakModel.Jmeno,
                    Prijmeni    = zakModel.Prijmeni,
                    Muz         = zakModel.Muz,
                    RodneCislo  = zakModel.RodneCislo,
                    Studuje     = zakModel.Studuje,
                    Trida       = zakModel.Trida,
                    ZmenenoKdy  = zakModel.ZmenenoKdy,
                    ZmenenoKdo  = zakModel.ZmenenoKdo
                });
            }

            ViewBag.PocetZaku = seznamZaku.Count;
            return View(seznamZaku);
        }

        [HttpGet]
        public ActionResult Create()
        {
            return View(new ZakLocalModel
            {
                ZmenenoKdo  = "(⌐■_■)"
            });
        }

        [HttpPost]
        public ActionResult Create(ZakLocalModel zakLocalModel)
        {
            new SkolaSoapClient().ZaciCreate(new ZakModel
            {
                Id          = Guid.NewGuid(),
                Jmeno       = zakLocalModel.Jmeno,
                Prijmeni    = zakLocalModel.Prijmeni,
                Muz         = zakLocalModel.Muz,
                RodneCislo  = zakLocalModel.RodneCislo,
                Studuje     = zakLocalModel.Studuje,
                Trida       = Guid.NewGuid(),
                ZmenenoKdy  = DateTime.Now,
                ZmenenoKdo  = zakLocalModel.ZmenenoKdo
            });

            return View(zakLocalModel);
        }

        public ActionResult Delete(string id)
        {
            new SkolaSoapClient().ZaciSmazat(id);
            return RedirectToAction("List");
        }
    }
}
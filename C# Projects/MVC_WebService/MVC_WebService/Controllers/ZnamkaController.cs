using MVC_WebService.Models;
using MVC_WebService.SkolaWS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web.Mvc;

namespace MVC_WebService.Controllers
{
    public class ZnamkaController : Controller
    {
        public ActionResult List(string zakId, string jmenoZaka)
        {
            SkolaSoapClient client  = new SkolaSoapClient();
            PredmetModel[] predmety = client.PredmetyList();

            List<ZnamkaLocalModel> list = new List<ZnamkaLocalModel>();

            foreach (ZnamkaModel znamkaModel in client.ZnamkyList(zakId))
            {
                list.Add(new ZnamkaLocalModel
                {
                    Id              = znamkaModel.Id,
                    Predmet         = znamkaModel.Predmet,
                    Zak             = znamkaModel.Zak,
                    Znamka          = znamkaModel.Znamka,
                    PredmetNazev    = predmety.First(p => p.Id == znamkaModel.Predmet).Predmet
                });
            }

            ViewBag.JmenoZaka   = jmenoZaka;
            ViewBag.ZakId       = zakId;

            return View(list);
        }

        [HttpGet]
        public ActionResult Create(string zakId, string jmenoZaka)
        {
            ViewBag.ZakId               = zakId;
            ViewBag.JmenoZaka           = jmenoZaka;
            ViewBag.PredmetySelectList  = CreatePredmetySelectList();

            return View(new ZnamkaLocalModel
            {
                Id  = Guid.NewGuid(),
                Zak = Guid.Parse(zakId)
            });
        }
        
        [HttpPost]
        public ActionResult Create(ZnamkaLocalModel znamkaLocalModel, string zakId, string jmenoZaka)
        {
            ViewBag.ZakId               = zakId;
            ViewBag.JmenoZaka           = jmenoZaka;
            ViewBag.PredmetySelectList  = CreatePredmetySelectList();

            new SkolaSoapClient().ZnamkaCreate(znamkaLocalModel.Znamka, znamkaLocalModel.Zak, znamkaLocalModel.Predmet);
            return View(znamkaLocalModel);
        }

        private IEnumerable<SelectListItem> CreatePredmetySelectList()
        {
            foreach (PredmetModel predmet in new SkolaSoapClient().PredmetyList())
            {
                yield return new SelectListItem
                {
                    Text = predmet.Predmet,
                    Value = predmet.Id.ToString()
                };
            }
        }

        public ActionResult Delete(string znamkaId, string zakId, string jmenoZaka)
        {
            new SkolaSoapClient().ZnamkaDelete(znamkaId);
            return RedirectToAction("List", new { zakId=zakId, jmenoZaka=jmenoZaka });
        }
    }
}
import React, { useContext, useState, useEffect } from "react";
import {
  Card, Row, Col, Button, Container, Modal, Form, Table, Tabs, Tab
} from "react-bootstrap";
import jsPDF from "jspdf";
import "jspdf-autotable";


import { HOST, PORT, DOMAIN } from '../config';
import { teacherContext } from './App';

const styles = {
    profile: {position: "fixed", top: "300px", left: "100px" },
    work: {marginLeft: "150px"},
    avatar: { width: "150px", height: "150px", display: "block", margin: "0 auto" },
    workinfo: {display: "flex", justifyContent: "space-between", alignItems: "center", marginBottom: "20px"},
    workTitle: {fontSize: "36px", fontWeight: "bold", textAlign: "center", margin: 0},
    worklist: {marginBottom: "50px"},
    button: { marginRight: "10px" }
}

const ProfileInfo = (props) => {
    const { teacher } = props;

    const genderMap = {
      1: "男",
      2: "女",
    };
    
    const jobMap = {
      1: "博士后",
      2: "助教",
      3: "讲师",
      4: "副教授",
      5: "特任教授",
      6: "教授",
      7: "助理研究员",
      8: "特任副研究员",
      9: "副研究员",
      10: "特任研究员",
      11: "研究员",
    };

    return (
      <Card>
        <Card.Header className="text-center">教师信息</Card.Header>
        <Card.Img
          variant="top"
          src={teacher.avatar ? `/upload/${teacher.avatar}` : "/img/avatar.png"}
          className="rounded-circle img-thumbnail"
          style={styles.avatar}
        />
        <Card.Body>
          <Row>
              <Col sm={4}>
              <Card.Title>工号</Card.Title>
              </Col>
              <Col sm={8}>
              <Card.Text className="text-center">{teacher.tid}</Card.Text>
              </Col>
          </Row>
          <Row>
              <Col sm={4}>
              <Card.Title>姓名</Card.Title>
              </Col>
              <Col sm={8}>
              <Card.Text className="text-center">{teacher.tname}</Card.Text>
              </Col>
          </Row>
          <Row>
              <Col sm={4}>
              <Card.Title>性别</Card.Title>
              </Col>
              <Col sm={8}>
              <Card.Text className="text-center">{genderMap[teacher.gender]}</Card.Text>
              </Col>
          </Row>
          <Row>
              <Col sm={4}>
              <Card.Title>职称</Card.Title>
              </Col>
              <Col sm={8}>
              <Card.Text className="text-center">{jobMap[teacher.job]}</Card.Text>
              </Col>
          </Row>
          </Card.Body>
      </Card>
    );
};

const MyLessonRow = (props) => {
  const { lesson } = props;
  const termMap = {
    "": "无",
    1: "春季学期",
    2: "夏季学期",
    3: "秋季学期",
  }
  const courseNatureMap = {
    "": "无",
    1: "本科生课程",
    2: "研究生课程"
  }
  return (
    <tr>
      <td>{lesson.lid}</td>
      <td>{lesson.lname}</td>
      <td>{lesson.creditHours}</td>
      <td>{courseNatureMap[lesson.courseNature]}</td>
      <td>{lesson.teachYear}</td>
      <td>{termMap[lesson.term]}</td>
      <td>{lesson.undertakeCreditHours}</td>
    </tr>
  );
};


const LessonList = (props) => {
  const {loadLessons, lessons} = props;
  useEffect(() => {loadLessons()},[]);
  const rows = lessons.map((lesson, idx) => (
    <MyLessonRow key={idx} lesson={lesson} />
  ));
  return (
    <Card>
      <Card.Title className="text-center"></Card.Title>
      <Card.Body>
        <Table bordered hover>
          <thead>
            <tr>
              <th>课程号</th>
              <th>课程名</th>
              <th>学时</th>
              <th>课程性质</th>
              <th>年份</th>
              <th>学期</th>
              <th>承担学时</th>
            </tr>
          </thead>
          <tbody>
            {rows}
          </tbody>
        </Table>
      </Card.Body>
    </Card>
  );
};

const MyPaperRow = (props) => {
  const { paper } = props;
  const publicTypeMap = {
    "": "",
    1: "full paper",
    2: "short paper",
    3: "poster paper",
    4: "demo paper",
  }
  const publicRankMap = {
    "": "无",
    1: "CCF-A",
    2: "CCF-B",
    3: "CCF-C",
    4: "中文CCF-A",
    5: "中文CCF-B",
    6: "无级别",
  }
  const isCorrespondingAuthorMap = {
      true: "是",
      false: "否"
  }
  return (
    <tr>
      <td>{paper.sn}</td>
      <td>{paper.title}</td>
      <td>{paper.publicSource}</td>
      <td>{paper.publicationYear}</td>
      <td>{publicTypeMap[paper.publicType]}</td>
      <td>{publicRankMap[paper.publicRank]}</td>
      <td>{paper.paperRank}</td>
      <td>{isCorrespondingAuthorMap[Boolean(paper.isCorrespondingAuthor)]}</td>
    </tr>
  );
};

const PaperList = (props) => {
  const {loadPapers, papers} = props;
  useEffect(() => {loadPapers()}, []);
  const rows = papers.map((paper, idx) => (
    <MyPaperRow key={idx} paper={paper} />
  ));
  return (
    <Card>
      <Card.Title className="text-center"></Card.Title>
      <Card.Body>
        <Table bordered hover>
          <thead>
            <tr>
              <th>论文号</th>
              <th>论文名称</th>
              <th>发表源</th>
              <th>发表年份</th>
              <th>类型</th>
              <th>级别</th>
              <th>排名</th>
              <th>是否通讯作者</th>
            </tr>
          </thead>
          <tbody>
            {rows}
          </tbody>
        </Table>
      </Card.Body>
    </Card>
  );
};

const MyProjectRow = (props) => {
  const { project } = props;

  const projectTypeMap = {
    "": "无",
    "1": "国家级项目",
    "2": "省部级项目",
    "3": "市厅级项目",
    "4": "企业合作项目",
    "5": "其他类型项目"
  }

  return (
    <tr>
      <td>{project.pid}</td>
      <td>{project.pname}</td>
      <td>{project.projectSource}</td>
      <td>{projectTypeMap[project.projectType]}</td>
      <td>{project.totalBudget}</td>
      <td>{project.startYear}</td>
      <td>{project.endYear}</td>
      <td>{project.projectRank}</td>
      <td>{project.undertakeFunding}</td>
    </tr>
  );
};

const ProjectList = (props) => {
  const {loadProjects, projects} = props;
  useEffect(() => {loadProjects()}, []);
  const rows = projects.map((project, idx) => (
    <MyProjectRow key={idx} project={project} />
  ));
  return (
    <Card>
      <Card.Title className="text-center"></Card.Title>
      <Card.Body>
        <Table bordered hover>
          <thead>
            <tr>
              <th>项目号</th>
              <th>项目名称</th>
              <th>项目来源</th>
              <th>项目类型</th>
              <th>总经费</th>
              <th>开始年份</th>
              <th>结束年份</th>
              <th>排名</th>
              <th>承担经费</th>
            </tr>
          </thead>
          <tbody>
            {rows}
          </tbody>
        </Table>
      </Card.Body>
    </Card>
  );
};

const WorksInfo = (props) => {

    const { teacher } = props;
    const [lessons, setLessons] = useState([]);
    const [papers, setPapers] = useState([]);
    const [projects, setProjects] = useState([]);

    const [mode, setMode] = useState('lesson');

    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);

    const handleExportPdf = () => {
      jsPDF.API.addFont("wqy-microhei", "/font/wqy-microhei.ttc")
      const doc = new jsPDF();
      doc.setFont('wqy-microhei');
      // 添加教授信息
      doc.setFontSize(16);
      doc.text("教师信息", 20, 20);
      doc.autoTable({
        startY: 30,
        head: [["工号", "姓名", "性别", "职称"]],
        body: [[teacher.tid, teacher.tname, teacher.gender, teacher.job]],
      });
    
      // 添加课程信息
      const lessonData = [];
      lessons.map((lesson) => {
        lessonData.push([lesson.name]);
      });
    
      // 添加项目信息
      const projectData = [];
      projects.map((project) => {
        projectData.push([project.name]);
      });
    
      // 添加论文信息
      const paperData = [];
      papers.map((paper) => {
        paperData.push([paper.title, paper.author, paper.journal, paper.date]);
      });
    
      // 添加分割线
      doc.setLineWidth(0.5);
      doc.line(20, 90, 190, 90);
    
      // 添加课程信息
      doc.setFontSize(16);
      doc.text("教授课程", 20, 100);
      doc.autoTable({
        startY: 110,
        head: [["课程名称"]],
        body: lessonData,
      });
    
      // 添加分割线
      doc.setLineWidth(0.5);
      doc.line(20, 170, 190, 170);
    
      // 添加项目信息
      doc.setFontSize(16);
      doc.text("承担项目", 20, 180);
      doc.autoTable({
        startY: 190,
        head: [["项目名称"]],
        body: projectData,
      });
    
      // 添加分割线
      doc.setLineWidth(0.5);
      doc.line(20, 250, 190, 250);
    
      // 添加论文信息
      doc.setFontSize(16);
      doc.text("发表论文", 20, 260);
      doc.autoTable({
        startY: 270,
        head: [["论文题目", "作者", "期刊", "发表日期"]],
        body: paperData,
      });
    
      doc.save("teacher_info.pdf");
    };

    const loadLessons = async (body) => {
      const {tid} = JSON.parse(await localStorage.getItem(DOMAIN));
      const lessonRes = await fetch(`${HOST}:${PORT}/api/lesson/summary/${tid}`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
      });
      const lessonResult = await lessonRes.json();
      if (lessonRes.ok) {
        setLessons(lessonResult.data);
      } else {
        alert(lessonResult.message);
      }
    }
    const  loadPapers = async (body) => {
      const {tid} = JSON.parse(await localStorage.getItem(DOMAIN));
      const paperRes = await fetch(`${HOST}:${PORT}/api/paper/summary/${tid}`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
      });
      const paperResult = await paperRes.json();
      if (paperRes.ok) {
        setPapers(paperResult.data);
      } else {
        alert(paperResult.message);
      }
    }

    const loadProjects = async (body) => {
      const {tid} = JSON.parse(await localStorage.getItem(DOMAIN));
      const projectRes = await fetch(`${HOST}:${PORT}/api/project/summary/${tid}`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
      });
      const projectResult = await projectRes.json();
      if (projectRes.ok) {
        setProjects(projectResult.data);
      } else {
        alert(projectResult.message);
      }
    }

    const load = async (body) => {
      await loadLessons(body);
      await loadPapers(body);
      await loadProjects(body);
    }

    const handleSetGap = async () => {
      
      const form = document.forms.timeForm;
      const startYear = form.startYear.value;
      const endYear = form.endYear.value;
      
      const body = {
        startYear, endYear
      }
      await load(body);
      closeModal();
    }

    return (
      <Container>
        <div style={styles.workinfo}>
          <h1 style={styles.workTitle}>教师科研工作汇总</h1>
          <div>
            <div>
              <Button variant="primary" onClick={() => handleExportPdf()} style={styles.button}>
                  导出 PDF
              </Button>
              <Button variant="primary" onClick={() => showModal()} style={styles.button}>
                设定时间范围
              </Button>
              <Modal show={show} onHide={() => closeModal()}>
                <Modal.Header closeButton>
                    <Modal.Title>设定时间范围</Modal.Title>
                </Modal.Header>
                <Modal.Body>
                    <Form id="timeForm">
                      <Form.Group controlId="startYear">
                        <Form.Label>开始年份</Form.Label>
                        <Form.Control type="text" placeholder="请输入开始年份"/>
                      </Form.Group>
                      <Form.Group controlId="endYear">
                          <Form.Label>结束年份</Form.Label>
                          <Form.Control type="text" placeholder="请输入结束年份"/>
                      </Form.Group>
                    </Form>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="secondary" onClick={() => closeModal() }>
                        关闭
                    </Button>
                    <Button variant="primary" onClick={() => handleSetGap()}>
                        设置
                    </Button>
                </Modal.Footer>
              </Modal>
            </div>
          </div>
        </div>
        <div>
          <Tabs
              id="homeTabs"
              activeKey={mode}
              onSelect={mode => setMode(mode)}
          >
              <Tab eventKey="lesson" title="教授课程">
                <LessonList loadLessons={loadLessons} lessons={lessons}/>
              </Tab>
              <Tab eventKey="paper" title="发表论文">
                <PaperList loadPapers={loadPapers} papers={papers}/>
              </Tab>
              <Tab eventKey="project" title="承担项目">
                <ProjectList loadProjects={loadProjects} projects={projects}/>
              </Tab>
          </Tabs>
        </div>
      </Container>
    )
}
  


const TeacherInfo = () => {
    const { teacher } = useContext(teacherContext);

    return (
      <Row>
        <Col sm={12} md={2} style={styles.profile}>
            <ProfileInfo teacher={teacher} />
        </Col>
        <Col sm={12} md={10} style={styles.work}>
            <WorksInfo teacher={teacher}/>
        </Col>
      </Row>
    );
};

export default TeacherInfo;
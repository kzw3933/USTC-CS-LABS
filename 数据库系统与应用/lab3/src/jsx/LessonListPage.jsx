import React, { useState, useEffect, useContext } from 'react';
import { Button, Container, Table, Modal, Form, Col } from 'react-bootstrap';

import { HOST, PORT, DOMAIN } from '../config';
import { teacherContext } from './App';

const PostForm = () => {
    const currentYear = new Date().getFullYear();
    const years = Array.from({length: 11}, (_, i) => currentYear - i);
    const options = years.map((year) => (
      <option key={year} value={year}>
          {year}
      </option>
    ));
    return (
      <Form id="postForm">
          <Form.Group controlId="lid">
              <Form.Label>课程号</Form.Label>
              <Form.Control type="text" placeholder="请输入课程号"/>
          </Form.Group>
          <Form.Group controlId="lname">
              <Form.Label>课程名</Form.Label>
              <Form.Control type="text" placeholder="请输入课程名"/>
          </Form.Group>
          <Form.Group controlId="creditHours">
              <Form.Label>学时</Form.Label>
              <Form.Control type="text" placeholder="请输入课程学时数"/>
          </Form.Group>
          <Form.Group controlId="courseNature">
              <Form.Label>课程性质</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">本科生课程</option>
                <option value="2">研究生课程</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="teachYear">
              <Form.Label>年份</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                {options}
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="term">
              <Form.Label>学期</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">春季学期</option>
                <option value="2">夏季学期</option>
                <option value="3">秋季学期</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="undertakeCreditHours">
              <Form.Label>承担学时</Form.Label>
              <Form.Control type="text" placeholder="请输入承担学时数"/>
          </Form.Group>
      </Form>
    );
}

const PostButton = (props) => {
    const {loadLessons} = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const post = async (body) => {
        try {
            if(body.undertakeCreditHours !== body.creditHours) {
              throw new Error('普通用户只支持新增个人课程(个人承担学时要等于课程总学时)');
            }
            const res = await fetch(`${HOST}:${PORT}/api/lesson`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(body)
            }) ;
            const result = await res.json();
            if(res.ok) {
                alert(result.message);
                closeModal();
                loadLessons();
            } else {
                alert(result.message);
            }
        } catch(err) {
            alert(err.message);
        } 
    }
    const handlePost = async () => {
        const {tid} = JSON.parse(await localStorage.getItem(DOMAIN));
        const form = document.forms.postForm;
        const lid = form.lid.value;
        const lname = form.lname.value;
        const creditHours = form.creditHours.value;
        const courseNature = form.courseNature.value;
        const teachYear = form.teachYear.value;
        const term = form.term.value;
        const undertakeCreditHours = form.undertakeCreditHours.value;
        const body = {
            lid, lname, creditHours, courseNature, teachYear, term, undertakeCreditHours, tid
        };
        post(body);
    }
    return (
        <div>
            <Button variant="outline-success" size="sm" onClick={() => showModal()}>
                新建课程
            </Button>
            <Modal show={show} onHide={() => closeModal()}>
                <Modal.Header closeButton>
                    <Modal.Title>新建课程</Modal.Title>
                </Modal.Header>
                <Modal.Body>
                    <PostForm/>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="secondary" onClick={() => closeModal() }>
                        关闭
                    </Button>
                    <Button variant="primary" onClick={() => handlePost()}>
                        提交
                    </Button>
                </Modal.Footer>
            </Modal>
        </div>
    );
}

const ModifyForm = (props) => {
    const { modifyLid, setModifyLid } = props;
    const {tid} = JSON.parse(localStorage.getItem(DOMAIN));
    
    const handleModifyLidChange = (event) => {
      setModifyLid(event.target.value);
    };
    const handleButtonClick = async () => {
      try {
        const res = await fetch(`${HOST}:${PORT}/api/lesson/${tid}/${modifyLid}`, { method: 'GET' });
        const result = await res.json();
        if (res.ok) {
          const form = document.forms.modifyForm;
          form.lid.value = result.data.lid;
          form.lname.value = result.data.lname;
          form.creditHours.value = result.data.creditHours;
          form.courseNature.value = result.data.courseNature;
          form.teachYear.value = result.data.teachYear;
          form.term.value = result.data.term;
          form.undertakeCreditHours.value = result.data.undertakeCreditHours;
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
      }
    };

    const currentYear = new Date().getFullYear();
    const years = Array.from({length: 11}, (_, i) => currentYear - i);
    const options = years.map((year) => (
      <option key={year} value={year}>
          {year}
      </option>
    ));
    return (
      <Form id="modifyForm">
          <Form.Group controlId="lid">
            <Form.Label>待修改课程的课程号</Form.Label>
            <div className="d-flex">
              <Form.Control type="text" placeholder="请输入待修改课程的课程号" value={modifyLid} onChange={handleModifyLidChange} />
              <Button variant="primary" className="ml-2" onClick={handleButtonClick}>载入</Button>
            </div>
          </Form.Group>
          <Form.Group controlId="lname">
              <Form.Label>课程名</Form.Label>
              <Form.Control type="text" placeholder="请输入课程名"/>
          </Form.Group>
          <Form.Group controlId="creditHours">
              <Form.Label>学时</Form.Label>
              <Form.Control type="text" placeholder="请输入课程学时数"/>
          </Form.Group>
          <Form.Group controlId="courseNature">
              <Form.Label>课程性质</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">本科生课程</option>
                <option value="2">研究生课程</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="teachYear">
              <Form.Label>年份</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                {options}
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="term">
              <Form.Label>学期</Form.Label>
              <Form.Control as="select">
                <option value="">无</option>
                <option value="1">春季学期</option>
                <option value="2">夏季学期</option>
                <option value="3">秋季学期</option>
              </Form.Control>
          </Form.Group>
          <Form.Group controlId="undertakeCreditHours">
              <Form.Label>承担学时</Form.Label>
              <Form.Control type="text" placeholder="请输入承担学时数"/>
          </Form.Group>
      </Form>
    );
}

const ModifyButton = (props) => {
    const { loadLessons } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [modifyLid, setModifyLid] = useState("");
    const modify = async (body) => {
      try {
        if(body.undertakeCreditHours !== body.creditHours) {
          throw new Error('普通用户只支持修改个人课程(个人承担学时要等于课程总学时)');
        }
        const res = await fetch(`${HOST}:${PORT}/api/lesson`, {
          method: 'PATCH',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(body),
        });
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          closeModal();
          loadLessons();
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
      }
    };
    const handleModify = async () => {
      const { tid } = JSON.parse(await localStorage.getItem(DOMAIN));
      const form = document.forms.modifyForm;
      const lname = form.lname.value;
      const creditHours = form.creditHours.value;
      const courseNature = form.courseNature.value;
      const teachYear =   form.teachYear.value;
      const term = form.term.value;      
      const undertakeCreditHours = form.undertakeCreditHours.value;
      const body = {
        modifyLid, lname, creditHours, courseNature, teachYear, term, undertakeCreditHours, tid
      };
      modify(body);
    };
    return (
      <div>
        <Button variant="outline-warning" size="sm" onClick={() => showModal()}>
                  修改
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
          <Modal.Header closeButton>
            <Modal.Title>修改课程</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <ModifyForm setModifyLid={setModifyLid} modifyLid={modifyLid}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="primary" onClick={() => handleModify()}>
                          提交
            </Button>
          </Modal.Footer>
        </Modal>
      </div>
  );
}

const DeleteForm = (props) => {
  const { deleteLid, setDeleteLid } = props; 
  const handleDeleteLidChange = (event) => {
    setDeleteLid(event.target.value);
  };

  return (
    <Form id="deleteForm">
        <Form.Group controlId="lid">
          <Form.Label>待删除课程的课程号</Form.Label>
            <Form.Control type="text" placeholder="请输入待删除课程的课程号" value={deleteLid} onChange={handleDeleteLidChange} />
        </Form.Group>
    </Form>
  );
}

const DeleteButton = (props) => {
    const { loadLessons } = props;
    const [show, setShow] = useState(false);
    const showModal = () => setShow(true);
    const closeModal = () => setShow(false);
    const [deleteLid, setDeleteLid] = useState("");
    const handle = async (body) => {
      try {
        const res = await fetch(
          `${HOST}:${PORT}/api/lesson`, {
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(body),
          },
        );
        const result = await res.json();
        if (res.ok) {
          alert(result.message);
          closeModal();
          loadLessons();
        } else {
          alert(result.message);
        }
      } catch (err) {
        alert(err.message);
      }
    };
    const handleDelete = async () => {
      if (confirm('确认删除该课程吗？')) {
        const { tid } = JSON.parse(localStorage.getItem(DOMAIN));
        const body = { deleteLid, tid};
        handle(body);
      }
    };
    return (
      <div>
        <Button variant="outline-warning" size="sm" onClick={() => showModal()}>
                  删除
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
          <Modal.Header closeButton>
            <Modal.Title>删除课程</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <DeleteForm setDeleteLid={setDeleteLid} deleteLid={deleteLid}/>
          </Modal.Body>
          <Modal.Footer>
            <Button variant="secondary" onClick={() => closeModal()}>
                          关闭
            </Button>
            <Button variant="primary" onClick={() => handleDelete()}>
                          删除
            </Button>
          </Modal.Footer>
        </Modal>
      </div>
    );
  }

const MyRow = (props) => {
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
        <td>{lesson.tid}</td>
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
      <MyRow key={idx} lesson={lesson} />
    ));
    return (
      <Table bordered hover>
        <thead>
          <tr>
            <th>课程号</th>
            <th>教师号</th>
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
    );
};



const LessonListTable = (props) => {
    
  const {lessons, loadLessons} = props;
  const { auth } = useContext(teacherContext);
  
  return (
    <Container>
      {auth && (
        <Col md={12} className="d-flex justify-content-between align-items-center mb-3">
          <PostButton loadLessons={loadLessons} />
          <SearchButton loadLessons={loadLessons} />
          <ModifyButton loadLessons={loadLessons} />
          <DeleteButton loadLessons={loadLessons} />
        </Col>
      )}
      <LessonList loadLessons={loadLessons} lessons={lessons} />
    </Container>
  );
}


const SearchButton = (props) => {
  const {loadLessons} = props;
  const [show, setShow] = useState(false);
  const showModal = () => setShow(true);
  const closeModal = () => setShow(false);

  const handleSearch = async () => {
    const form = document.forms.searchForm;
    const tid = form.tid.value;
    const lid = form.lid.value;
    const lname = form.lname.value;
    const creditHours = form.creditHours.value;
    const courseNature = form.courseNature.value;
    const teachYear = form.teachYear.value;
    const term = form.term.value;
    const undertakeCreditHours = form.undertakeCreditHours.value;
    const body = {
        lid, lname, creditHours, courseNature, teachYear, term, undertakeCreditHours, tid
    };
    await loadLessons(body);
    closeModal();
}

  const handleClear = async () => {
    const form = document.forms.searchForm;
    form.reset();
  }

  return (
    <div>
        <Button variant="outline-success" size="sm" onClick={() => showModal()}>
            搜索筛选
        </Button>
        <Modal show={show} onHide={() => closeModal()}>
            <Modal.Header closeButton>
                <Modal.Title>搜索筛选</Modal.Title>
            </Modal.Header>
            <Modal.Body>
                <SearchForm/>
            </Modal.Body>
            <Modal.Footer>
                <Button variant="secondary" onClick={() => handleClear() }>
                    清除
                </Button>
                <Button variant="secondary" onClick={() => closeModal() }>
                    关闭
                </Button>
                <Button variant="primary" onClick={() => handleSearch()}>
                    搜索
                </Button>
            </Modal.Footer>
        </Modal>
    </div>
);

}

const SearchForm = () => {

  const currentYear = new Date().getFullYear();
  const years = Array.from({length: 11}, (_, i) => currentYear - i);
  const options = years.map((year) => (
    <option key={year} value={year}>
        {year}
    </option>
  ));

  return (
      <Form id="searchForm">
        <Form.Group controlId="tid">
            <Form.Label>教师号</Form.Label>
            <Form.Control type="text" placeholder="请输入教师号"/>
        </Form.Group>
        <Form.Group controlId="lid">
            <Form.Label>课程号</Form.Label>
            <Form.Control type="text" placeholder="请输入课程号"/>
        </Form.Group>
        <Form.Group controlId="lname">
            <Form.Label>课程名</Form.Label>
            <Form.Control type="text" placeholder="请输入课程名"/>
        </Form.Group>
        <Form.Group controlId="creditHours">
            <Form.Label>学时</Form.Label>
            <Form.Control type="text" placeholder="请输入课程学时数"/>
        </Form.Group>
        <Form.Group controlId="courseNature">
            <Form.Label>课程性质</Form.Label>
            <Form.Control as="select">
              <option value="">无</option>
              <option value="1">本科生课程</option>
              <option value="2">研究生课程</option>
            </Form.Control>
        </Form.Group>
        <Form.Group controlId="teachYear">
            <Form.Label>年份</Form.Label>
            <Form.Control as="select">
              <option value="">无</option>
              {options}
            </Form.Control>
        </Form.Group>
        <Form.Group controlId="term">
            <Form.Label>学期</Form.Label>
            <Form.Control as="select">
              <option value="">无</option>
              <option value="1">春季学期</option>
              <option value="2">夏季学期</option>
              <option value="3">秋季学期</option>
            </Form.Control>
        </Form.Group>
        <Form.Group controlId="undertakeCreditHours">
            <Form.Label>承担学时</Form.Label>
            <Form.Control type="text" placeholder="请输入承担学时数"/>
        </Form.Group>
      </Form>
  );
}

const LessonListPage = () => {
  const [lessons, setLessons] = useState([]);

  const loadLessons = async (body) => {
    try {      
      const res = await fetch(`${HOST}:${PORT}/api/lesson/search`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
      });
      const result = await res.json();
      if (res.ok) {
        setLessons(result.data);
      } else {
        alert(result.message);
      }
    } catch (err) {
      alert(err.message);
    }
  };

  const styles = {
    form: {left: "50px",},
  }

  return (
    <div style={styles.form}>
      <LessonListTable lessons = {lessons} loadLessons = {loadLessons}/>
    </div>
      
  );
};

export default LessonListPage;